#include "splash.h"
#include "vga.h"
#include "pit.h"
#include "io.h"
#include <stdint.h>

#define SPLASH_VGA    ((volatile uint16_t*)0xB8000)
#define SPLASH_WIDTH  80
#define SPLASH_HEIGHT 25
#define BAR_WIDTH     50
#define BAR_ROW       14
#define ART_ROW       6
#define HOLD_MS       5000

static const char* const art[] = {
    "  ___  ____  ____ ___ _____ ",
    " / _ \\|  _ \\| __ )_ _|_   _|",
    "| | | | |_) |  _ \\| |  | |  ",
    "| |_| |  _ <| |_) | |  | |  ",
    " \\___/|_| \\_\\____/___| |_|  ",
};

static uint16_t cell(char c, uint8_t fg, uint8_t bg)
{
    return (uint16_t)(uint8_t)c | (uint16_t)((fg | (bg << 4)) << 8);
}

static void put_cell(int row, int col, char c, uint8_t fg, uint8_t bg)
{
    if (row < 0 || row >= SPLASH_HEIGHT || col < 0 || col >= SPLASH_WIDTH)
        return;
    SPLASH_VGA[row * SPLASH_WIDTH + col] = cell(c, fg, bg);
}

static int str_len(const char* s)
{
    int n = 0;
    while (s[n])
        n++;
    return n;
}

static void put_centered(int row, const char* s, uint8_t fg, uint8_t bg)
{
    int col = (SPLASH_WIDTH - str_len(s)) / 2;
    for (int i = 0; s[i]; i++)
        put_cell(row, col + i, s[i], fg, bg);
}

static void clear_screen(uint8_t bg)
{
    for (int i = 0; i < SPLASH_WIDTH * SPLASH_HEIGHT; i++)
        SPLASH_VGA[i] = cell(' ', VGA_LIGHT_GREY, bg);
}

static void cursor_disable(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

static void cursor_enable(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x0E);
    outb(0x3D4, 0x0B);
    outb(0x3D5, 0x0F);
}

void splash_show(void)
{
    int art_count = (int)(sizeof(art) / sizeof(art[0]));
    int bar_left = (SPLASH_WIDTH - (BAR_WIDTH + 2)) / 2;
    int interior = bar_left + 1;

    cursor_disable();
    clear_screen(VGA_BLACK);

    for (int i = 0; i < art_count; i++)
        put_centered(ART_ROW + i, art[i], VGA_LIGHT_GREEN, VGA_BLACK);

    put_centered(ART_ROW + art_count + 1, "Made by xRookieFight", VGA_DARK_GREY, VGA_BLACK);

    put_cell(BAR_ROW, bar_left, '[', VGA_LIGHT_GREY, VGA_BLACK);
    put_cell(BAR_ROW, interior + BAR_WIDTH, ']', VGA_LIGHT_GREY, VGA_BLACK);
    for (int i = 0; i < BAR_WIDTH; i++)
        put_cell(BAR_ROW, interior + i, ' ', VGA_LIGHT_GREY, VGA_DARK_GREY);

    pit_sleep(HOLD_MS);

    static const struct {
        int target;
        int step_ms;
        int pause_ms;
    } seg[] = {
        { 8, 20, 400 },
        { 12, 90, 700 },
        { 26, 30, 300 },
        { 30, 120, 1200 },
        { 40, 25, 250 },
        { 41, 200, 1500 },
        { 50, 15, 0 },
    };
    int seg_count = (int)(sizeof(seg) / sizeof(seg[0]));
    int filled = 0;
    for (int s = 0; s < seg_count; s++) {
        while (filled < seg[s].target) {
            put_cell(BAR_ROW, interior + filled, ' ', VGA_BLACK, VGA_LIGHT_GREEN);
            filled++;
            pit_sleep(seg[s].step_ms);
        }
        if (seg[s].pause_ms)
            pit_sleep(seg[s].pause_ms);
    }

    pit_sleep(250);
    cursor_enable();
}
