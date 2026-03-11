#include "window_manager.h"
#include "../gui/render.h"

#define MAX_WINDOWS 8

static Window windows[MAX_WINDOWS];
static u32 window_count;

static int rect_contains(WindowRect* r, s32 x, s32 y)
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

void window_manager_init(void)
{
    window_count = 0;
}

Window* window_manager_create_window(u32 app_id, const char* title, s32 x, s32 y, s32 w, s32 h)
{
    if (window_count >= MAX_WINDOWS)
    {
        return 0;
    }
    Window* win = &windows[window_count++];
    win->id = window_count;
    win->rect.x = x;
    win->rect.y = y;
    win->rect.width = w;
    win->rect.height = h;
    win->title = title;
    win->app_id = app_id;
    win->visible = true;
    win->focused = true;
    win->dragging = false;
    win->drag_offset_x = 0;
    win->drag_offset_y = 0;
    return win;
}

static void focus_window(Window* win)
{
    for (u32 i = 0; i < window_count; i++)
    {
        windows[i].focused = false;
    }
    win->focused = true;
}

void window_manager_handle_event(const Event* e)
{
    if (e->type == EVENT_MOUSE_BUTTON)
    {
        if (e->pressed)
        {
            for (s32 i = (s32)window_count - 1; i >= 0; i--)
            {
                Window* win = &windows[i];
                if (!win->visible)
                {
                    continue;
                }
                WindowRect header;
                header.x = win->rect.x;
                header.y = win->rect.y;
                header.width = win->rect.width;
                header.height = 20;
                if (rect_contains(&header, e->x, e->y))
                {
                    focus_window(win);
                    win->dragging = true;
                    win->drag_offset_x = e->x - win->rect.x;
                    win->drag_offset_y = e->y - win->rect.y;
                    break;
                }
            }
        }
        else
        {
            for (u32 i = 0; i < window_count; i++)
            {
                windows[i].dragging = false;
            }
        }
    }
    if (e->type == EVENT_MOUSE_MOVE)
    {
        for (u32 i = 0; i < window_count; i++)
        {
            Window* win = &windows[i];
            if (win->dragging)
            {
                win->rect.x = e->x - win->drag_offset_x;
                win->rect.y = e->y - win->drag_offset_y;
            }
        }
    }
}

Window* window_manager_get_windows(u32* count)
{
    *count = window_count;
    return windows;
}

void window_manager_render(Framebuffer* fb)
{
    for (u32 i = 0; i < window_count; i++)
    {
        Window* win = &windows[i];
        if (!win->visible)
        {
            continue;
        }
        u32 border_color = win->focused ? 0xFFFFFF : 0x808080;
        u32 header_color = win->focused ? 0x305080 : 0x202840;
        u32 body_color = 0x181820;
        render_rect(fb, (u32)win->rect.x, (u32)win->rect.y, (u32)win->rect.width, (u32)win->rect.height, border_color);
        render_rect(fb, (u32)win->rect.x + 1, (u32)win->rect.y + 1, (u32)win->rect.width - 2, 18, header_color);
        render_rect(fb, (u32)win->rect.x + 1, (u32)win->rect.y + 19, (u32)win->rect.width - 2, (u32)win->rect.height - 20, body_color);
        render_text(fb, (u32)win->rect.x + 6, (u32)win->rect.y + 4, win->title, 0xFFFFFF, header_color);
    }
}

