#include "desktop.h"
#include "../lib/string.h"

static DesktopIcon desktop_icons[3];
static u32 desktop_icon_count;

static Rect make_rect(s32 x, s32 y, s32 w, s32 h)
{
    Rect r;
    r.x = x;
    r.y = y;
    r.width = w;
    r.height = h;
    return r;
}

static int rect_contains(const Rect* r, s32 x, s32 y)
{
    if (x < r->x)
    {
        return 0;
    }
    if (y < r->y)
    {
        return 0;
    }
    if (x >= r->x + r->width)
    {
        return 0;
    }
    if (y >= r->y + r->height)
    {
        return 0;
    }
    return 1;
}

void desktop_init(Framebuffer* fb)
{
    desktop_icon_count = 0;
    framebuffer_clear(fb, 0x202840);
}

Rect desktop_get_taskbar_rect(Framebuffer* fb)
{
    return make_rect(0, (s32)fb->height - 40, (s32)fb->width, 40);
}

u32 desktop_get_icon_count(void)
{
    return desktop_icon_count;
}

Rect desktop_get_icon_rect(u32 index)
{
    if (index >= desktop_icon_count)
    {
        return make_rect(0, 0, 0, 0);
    }
    return desktop_icons[index].area;
}

const DesktopIcon* desktop_get_icons(void)
{
    return desktop_icons;
}

bool desktop_handle_event(const Event* e)
{
    return false;
}

void desktop_render(Framebuffer* fb)
{
    framebuffer_clear(fb, 0x1B2836);

    for (u32 y = 0; y < fb->height; y += 40)
    {
        for (u32 x = 0; x < fb->width; x += 40)
        {
            u32 c = ((x + y) / 40) % 2 ? 0x1F3342 : 0x1B2836;
            render_rect(fb, x, y, 40, 40, c);
        }
    }

    Rect taskbar = desktop_get_taskbar_rect(fb);
    render_rect(fb, (u32)taskbar.x, (u32)taskbar.y, (u32)taskbar.width, (u32)taskbar.height, 0x151A22);
    render_rect(fb, 0, (u32)taskbar.y, 140, (u32)taskbar.height, 0x181F28);
    render_text(fb, 16, (u32)taskbar.y + 12, "Orbit", 0xFFFFFF, 0x181F28);

    render_text(fb, 160, (u32)taskbar.y + 12, "Activities  Files  Settings", 0xA0B0C0, 0x151A22);
}

