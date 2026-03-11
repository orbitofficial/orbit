#include "apps.h"
#include "../gui/render.h"

static Window* files_window;
static Window* settings_window;
static Window* demo_window;

void apps_init(void)
{
    files_window = 0;
    settings_window = 0;
    demo_window = 0;
}

void apps_on_launch(u32 app_id)
{
    if (app_id == 1)
    {
        if (!files_window)
        {
            files_window = window_manager_create_window(1, "Orbit Files", 220, 80, 360, 260);
        }
    }
    if (app_id == 2)
    {
        if (!settings_window)
        {
            settings_window = window_manager_create_window(2, "Orbit Settings", 260, 120, 320, 220);
        }
    }
    if (app_id == 3)
    {
        if (!demo_window)
        {
            demo_window = window_manager_create_window(3, "Orbit Demo", 300, 160, 320, 220);
        }
    }
}

static void render_files(Window* win, Framebuffer* fb)
{
    u32 x = (u32)win->rect.x;
    u32 y = (u32)win->rect.y + 20;
    u32 w = (u32)win->rect.width;
    u32 h = (u32)win->rect.height - 20;
    render_rect(fb, x + 1, y + 1, w - 2, h - 2, 0x181820);

    u32 sidebar_w = 120;
    render_rect(fb, x + 4, y + 4, sidebar_w - 8, h - 12, 0x202530);
    render_text(fb, x + 12, y + 12, "Places", 0xFFFFFF, 0x202530);
    render_text(fb, x + 12, y + 32, "Home", 0xFFFFFF, 0x202530);
    render_text(fb, x + 12, y + 48, "Documents", 0xFFFFFF, 0x202530);
    render_text(fb, x + 12, y + 64, "Pictures", 0xFFFFFF, 0x202530);
    render_text(fb, x + 12, y + 80, "Apps", 0xFFFFFF, 0x202530);

    u32 list_x = x + sidebar_w + 8;
    render_rect(fb, list_x, y + 4, w - sidebar_w - 16, h - 12, 0x101015);
    render_text(fb, list_x + 8, y + 12, "Name", 0xAAAAAA, 0x101015);
    render_text(fb, list_x + 120, y + 12, "Type", 0x555555, 0x101015);
    render_text(fb, list_x + 8, y + 32, "orbit.txt", 0xFFFFFF, 0x101015);
    render_text(fb, list_x + 120, y + 32, "Text", 0x777777, 0x101015);
}

static void render_settings(Window* win, Framebuffer* fb)
{
    u32 x = (u32)win->rect.x;
    u32 y = (u32)win->rect.y + 20;
    u32 w = (u32)win->rect.width;
    u32 h = (u32)win->rect.height - 20;
    render_rect(fb, x + 1, y + 1, w - 2, h - 2, 0x181820);

    render_text(fb, x + 16, y + 16, "Orbit System", 0xFFFFFF, 0x181820);
    render_text(fb, x + 16, y + 40, "Name:", 0xAAAAAA, 0x181820);
    render_text(fb, x + 80, y + 40, "Orbit", 0xFFFFFF, 0x181820);
    render_text(fb, x + 16, y + 60, "Version:", 0xAAAAAA, 0x181820);
    render_text(fb, x + 80, y + 60, "0.1", 0xFFFFFF, 0x181820);
}

static void render_demo(Window* win, Framebuffer* fb)
{
    u32 x = (u32)win->rect.x;
    u32 y = (u32)win->rect.y + 20;
    u32 w = (u32)win->rect.width;
    u32 h = (u32)win->rect.height - 20;
    render_rect(fb, x + 1, y + 1, w - 2, h - 2, 0x181820);

    u32 cx = x + w / 2 - 80;
    u32 cy = y + h / 2 - 20;
    render_rect(fb, cx, cy, 160, 40, 0x50A0FF);
    render_text(fb, cx + 20, cy + 12, "Welcome to Orbit", 0x000000, 0x50A0FF);
}

void apps_render(Framebuffer* fb)
{
    u32 count;
    Window* wins = window_manager_get_windows(&count);
    for (u32 i = 0; i < count; i++)
    {
        Window* w = &wins[i];
        if (!w->visible)
        {
            continue;
        }
        if (w->app_id == 1)
        {
            render_files(w, fb);
        }
        if (w->app_id == 2)
        {
            render_settings(w, fb);
        }
        if (w->app_id == 3)
        {
            render_demo(w, fb);
        }
    }
}

void apps_handle_event(const Event* e)
{
    (void)e;
}

