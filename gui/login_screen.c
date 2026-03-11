#include "login_screen.h"
#include "render.h"

static s32 button_x;
static s32 button_y;
static s32 button_w;
static s32 button_h;

static int point_in_button(s32 x, s32 y)
{
    if (x < button_x)
    {
        return 0;
    }
    if (y < button_y)
    {
        return 0;
    }
    if (x >= button_x + button_w)
    {
        return 0;
    }
    if (y >= button_y + button_h)
    {
        return 0;
    }
    return 1;
}

void login_screen_init(Framebuffer* fb)
{
    u32 w = fb->width;
    u32 h = fb->height;
    u32 panel_w = 360;
    u32 panel_h = 220;
    u32 px = (w - panel_w) / 2;
    u32 py = (h - panel_h) / 2;
    button_w = 120;
    button_h = 32;
    button_x = (s32)(px + panel_w - button_w - 24);
    button_y = (s32)(py + panel_h - button_h - 24);
}

void login_screen_render(Framebuffer* fb)
{
    framebuffer_clear(fb, 0x10141C);
    u32 w = fb->width;
    u32 h = fb->height;
    u32 panel_w = 360;
    u32 panel_h = 220;
    u32 px = (w - panel_w) / 2;
    u32 py = (h - panel_h) / 2;
    render_rect(fb, px, py, panel_w, panel_h, 0x181C24);
    render_rect(fb, px + 1, py + 1, panel_w - 2, panel_h - 2, 0x202634);

    render_text(fb, px + 24, py + 28, "Orbit", 0xFFFFFF, 0x202634);
    render_text(fb, px + 24, py + 44, "Welcome", 0xA0B0C0, 0x202634);

    render_text(fb, px + 24, py + 80, "User", 0xC0C8D0, 0x202634);
    render_rect(fb, px + 24, py + 96, panel_w - 48, 24, 0x121620);

    render_text(fb, px + 24, py + 132, "Password", 0xC0C8D0, 0x202634);
    render_rect(fb, px + 24, py + 148, panel_w - 48, 24, 0x121620);

    render_rect(fb, (u32)button_x, (u32)button_y, (u32)button_w, (u32)button_h, 0x3A9AFE);
    render_text(fb, (u32)button_x + 30, (u32)button_y + 10, "Log In", 0xFFFFFF, 0x3A9AFE);
}

bool login_screen_handle_event(const Event* e)
{
    if (e->type == EVENT_MOUSE_BUTTON && e->pressed)
    {
        if (point_in_button(e->x, e->y))
        {
            return true;
        }
    }
    return false;
}

