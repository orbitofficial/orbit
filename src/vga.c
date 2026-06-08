#include "vga.h"
#include "io.h"
#include "string.h"

#define VGA_MEMORY ((volatile uint16_t*)0xB8000)
#define VGA_FONT   ((volatile uint8_t*)0xA0000)
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MAX_HEIGHT 50
#define SCROLLBACK_LINES 200
#define SCROLL_STEP (vga_height - 1)

static size_t cursor_row;
static size_t cursor_col;
static uint8_t color_attr;
static size_t vga_height = VGA_HEIGHT;

static uint8_t saved_font[256 * 16];
static int font_saved;

static uint16_t history[SCROLLBACK_LINES * VGA_WIDTH];
static size_t history_count;
static size_t history_head;
static size_t view_offset;
static uint16_t live_snapshot[VGA_WIDTH * VGA_MAX_HEIGHT];

static uint16_t entry(char c, uint8_t attr)
{
    return (uint16_t)c | ((uint16_t)attr << 8);
}

static void move_cursor(void)
{
    uint16_t pos = (uint16_t)(cursor_row * VGA_WIDTH + cursor_col);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void history_push(volatile uint16_t* line)
{
    for (size_t x = 0; x < VGA_WIDTH; x++)
        history[history_head * VGA_WIDTH + x] = line[x];
    history_head = (history_head + 1) % SCROLLBACK_LINES;
    if (history_count < SCROLLBACK_LINES)
        history_count++;
}

static void history_render(void)
{
    for (size_t r = 0; r < vga_height; r++) {
        size_t s = history_count - view_offset + r;
        if (s < history_count) {
            size_t idx = (history_head + SCROLLBACK_LINES - history_count + s) % SCROLLBACK_LINES;
            for (size_t x = 0; x < VGA_WIDTH; x++)
                VGA_MEMORY[r * VGA_WIDTH + x] = history[idx * VGA_WIDTH + x];
        } else {
            size_t row = s - history_count;
            for (size_t x = 0; x < VGA_WIDTH; x++)
                VGA_MEMORY[r * VGA_WIDTH + x] = live_snapshot[row * VGA_WIDTH + x];
        }
    }
}

static void view_restore(void)
{
    if (view_offset == 0)
        return;
    view_offset = 0;
    for (size_t i = 0; i < VGA_WIDTH * vga_height; i++)
        VGA_MEMORY[i] = live_snapshot[i];
    move_cursor();
}

void vga_scroll_up(void)
{
    if (history_count == 0)
        return;
    if (view_offset == 0)
        for (size_t i = 0; i < VGA_WIDTH * vga_height; i++)
            live_snapshot[i] = VGA_MEMORY[i];
    if (view_offset >= history_count)
        return;
    view_offset += SCROLL_STEP;
    if (view_offset > history_count)
        view_offset = history_count;
    history_render();
}

void vga_scroll_down(void)
{
    if (view_offset == 0)
        return;
    if (view_offset <= SCROLL_STEP)
        view_offset = 0;
    else
        view_offset -= SCROLL_STEP;
    if (view_offset == 0)
        view_restore();
    else
        history_render();
}

static void scroll(void)
{
    if (cursor_row < vga_height)
        return;
    history_push(&VGA_MEMORY[0]);
    for (size_t y = 1; y < vga_height; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];
    for (size_t x = 0; x < VGA_WIDTH; x++)
        VGA_MEMORY[(vga_height - 1) * VGA_WIDTH + x] = entry(' ', color_attr);
    cursor_row = vga_height - 1;
}

void vga_set_color(uint8_t fg, uint8_t bg)
{
    color_attr = (uint8_t)(fg | (bg << 4));
}

void vga_clear(void)
{
    for (size_t i = 0; i < VGA_WIDTH * vga_height; i++)
        VGA_MEMORY[i] = entry(' ', color_attr);
    cursor_row = 0;
    cursor_col = 0;
    view_offset = 0;
    move_cursor();
}

void vga_init(void)
{
    color_attr = (uint8_t)(VGA_LIGHT_GREY | (VGA_BLACK << 4));
    vga_clear();
}

void vga_putc(char c)
{
    view_restore();
    switch (c) {
    case '\n':
        cursor_col = 0;
        cursor_row++;
        break;
    case '\r':
        cursor_col = 0;
        break;
    case '\b':
        if (cursor_col > 0) {
            cursor_col--;
            VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = entry(' ', color_attr);
        }
        break;
    case '\t':
        cursor_col = (cursor_col + 4) & ~3u;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
        break;
    default:
        VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = entry(c, color_attr);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
        break;
    }
    scroll();
    move_cursor();
}

void vga_write(const char* s)
{
    while (*s)
        vga_putc(*s++);
}

static void font_plane_begin(void)
{
    outb(0x3C4, 0x00);
    outb(0x3C5, 0x01);
    outb(0x3C4, 0x02);
    outb(0x3C5, 0x04);
    outb(0x3C4, 0x04);
    outb(0x3C5, 0x07);
    outb(0x3C4, 0x00);
    outb(0x3C5, 0x03);
    outb(0x3CE, 0x04);
    outb(0x3CF, 0x02);
    outb(0x3CE, 0x05);
    outb(0x3CF, 0x00);
    outb(0x3CE, 0x06);
    outb(0x3CF, 0x00);
}

static void font_plane_end(void)
{
    outb(0x3C4, 0x00);
    outb(0x3C5, 0x01);
    outb(0x3C4, 0x02);
    outb(0x3C5, 0x03);
    outb(0x3C4, 0x04);
    outb(0x3C5, 0x03);
    outb(0x3C4, 0x00);
    outb(0x3C5, 0x03);
    outb(0x3CE, 0x04);
    outb(0x3CF, 0x00);
    outb(0x3CE, 0x05);
    outb(0x3CF, 0x10);
    outb(0x3CE, 0x06);
    outb(0x3CF, 0x0E);
}

static void font_save(void)
{
    font_plane_begin();
    for (int c = 0; c < 256; c++)
        for (int r = 0; r < 16; r++)
            saved_font[c * 16 + r] = VGA_FONT[c * 32 + r];
    font_plane_end();
    font_saved = 1;
}

static void font_apply16(void)
{
    font_plane_begin();
    for (int c = 0; c < 256; c++) {
        for (int r = 0; r < 16; r++)
            VGA_FONT[c * 32 + r] = saved_font[c * 16 + r];
        for (int r = 16; r < 32; r++)
            VGA_FONT[c * 32 + r] = 0;
    }
    font_plane_end();
}

static void font_apply8(void)
{
    font_plane_begin();
    for (int c = 0; c < 256; c++) {
        for (int r = 0; r < 8; r++)
            VGA_FONT[c * 32 + r] =
                (uint8_t)(saved_font[c * 16 + r * 2] | saved_font[c * 16 + r * 2 + 1]);
        for (int r = 8; r < 32; r++)
            VGA_FONT[c * 32 + r] = 0;
    }
    font_plane_end();
}

static void crtc_set_cell(int height)
{
    outb(0x3D4, 0x09);
    uint8_t msl = inb(0x3D5);
    msl = (uint8_t)((msl & 0xE0) | ((height - 1) & 0x1F));
    outb(0x3D4, 0x09);
    outb(0x3D5, msl);

    outb(0x3D4, 0x0A);
    outb(0x3D5, (uint8_t)(height - 2));
    outb(0x3D4, 0x0B);
    outb(0x3D5, (uint8_t)(height - 1));
}

int vga_cols(void)
{
    return VGA_WIDTH;
}

int vga_rows(void)
{
    return (int)vga_height;
}

int vga_set_resolution(int cols, int rows)
{
    if (cols != VGA_WIDTH || (rows != 25 && rows != 50))
        return -1;

    cli();
    if (!font_saved)
        font_save();
    if (rows == 50) {
        font_apply8();
        crtc_set_cell(8);
    } else {
        font_apply16();
        crtc_set_cell(16);
    }
    vga_height = (size_t)rows;
    sti();

    history_count = 0;
    history_head = 0;
    view_offset = 0;
    vga_clear();
    return 0;
}
