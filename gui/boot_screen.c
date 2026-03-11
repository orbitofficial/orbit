#include "boot_screen.h"
#include "render.h"

static u32 boot_done;

void boot_screen_init(Framebuffer* fb)
{
    (void)fb;
    boot_done = 0;
}

void boot_screen_render(Framebuffer* fb)
{
    framebuffer_clear(fb, 0x102030);
    u32 w = fb->width;
    u32 h = fb->height;
    u32 panel_w = 420;
    u32 panel_h = 180;
    u32 px = (w - panel_w) / 2;
    u32 py = (h - panel_h) / 2;
    render_rect(fb, px, py, panel_w, panel_h, 0x1C2430);
    render_rect(fb, px + 1, py + 1, panel_w - 2, panel_h - 2, 0x202A36);
    render_text(fb, px + 24, py + 40, "Orbit", 0xFFFFFF, 0x202A36);
    render_text(fb, px + 24, py + 64, "A graphical OS for experiments", 0xB0C0D0, 0x202A36);
    u32 bar_y = py + panel_h - 40;
    u32 bar_x = px + 24;
    u32 bar_w = panel_w - 48;
    render_rect(fb, bar_x, bar_y, bar_w, 8, 0x151A22);
    render_rect(fb, bar_x, bar_y, bar_w / 3, 8, 0x3A9AFE);
    render_text(fb, bar_x, bar_y - 16, "Booting Orbit...", 0xA0B0C0, 0x202A36);
}

bool boot_screen_handle_event(const Event* e)
{
    if (e->type == EVENT_KEY_DOWN || e->type == EVENT_MOUSE_BUTTON)
    {
        boot_done = 1;
    }
    return boot_done != 0;
}

