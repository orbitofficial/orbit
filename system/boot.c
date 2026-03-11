#include "boot.h"
#include "../core/event.h"
#include "../core/window_manager.h"
#include "../drivers/mouse.h"
#include "../drivers/keyboard.h"
#include "../gui/desktop.h"
#include "../gui/render.h"
#include "../gui/boot_screen.h"
#include "../gui/login_screen.h"

static s32 cursor_x;
static s32 cursor_y;

typedef enum
{
    UI_BOOT = 0,
    UI_LOGIN = 1,
    UI_DESKTOP = 2
} UiState;

static UiState ui_state;

static void render_cursor(Framebuffer* fb)
{
    u32 color = 0xFFFFFF;
    s32 size = 12;
    if (cursor_x < 0 || cursor_y < 0)
    {
        return;
    }
    if (cursor_x + size >= (s32)fb->width)
    {
        size = (s32)fb->width - cursor_x - 1;
    }
    if (cursor_y + size >= (s32)fb->height)
    {
        size = (s32)fb->height - cursor_y - 1;
    }
    if (size <= 0)
    {
        return;
    }
    for (s32 dy = 0; dy < size; dy++)
    {
        for (s32 dx = 0; dx <= dy; dx++)
        {
            framebuffer_put_pixel(fb, (u32)(cursor_x + dx), (u32)(cursor_y + dy), color);
        }
    }
}

void system_boot(Framebuffer* fb)
{
    event_queue_init();
    window_manager_init();
    mouse_init();
    mouse_set_bounds((s32)fb->width - 1, (s32)fb->height - 1);
    keyboard_init();
    desktop_init(fb);
    boot_screen_init(fb);
    login_screen_init(fb);
    ui_state = UI_BOOT;
    cursor_x = fb->width / 2;
    cursor_y = fb->height / 2;

    while (1)
    {
        bool dirty = false;

        mouse_poll();
        keyboard_poll();

        Event e;
        while (event_queue_pop(&e))
        {
            dirty = true;
            if (e.type == EVENT_MOUSE_MOVE)
            {
                cursor_x = e.x;
                cursor_y = e.y;
            }
            if (ui_state == UI_BOOT)
            {
                if (boot_screen_handle_event(&e))
                {
                    ui_state = UI_LOGIN;
                }
            }
            else if (ui_state == UI_LOGIN)
            {
                if (login_screen_handle_event(&e))
                {
                    ui_state = UI_DESKTOP;
                }
            }
            else if (ui_state == UI_DESKTOP)
            {
                window_manager_handle_event(&e);
                desktop_handle_event(&e);
            }
        }

        if (dirty)
        {
            if (ui_state == UI_BOOT)
            {
                boot_screen_render(fb);
            }
            else if (ui_state == UI_LOGIN)
            {
                login_screen_render(fb);
            }
            else if (ui_state == UI_DESKTOP)
            {
                desktop_render(fb);
                window_manager_render(fb);
            }
            render_cursor(fb);
        }
    }
}

