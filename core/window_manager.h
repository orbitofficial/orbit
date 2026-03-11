#ifndef ORBIT_WINDOW_MANAGER_H
#define ORBIT_WINDOW_MANAGER_H

#include "../gui/framebuffer.h"
#include "../core/event.h"
#include "../lib/types.h"

typedef struct
{
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} WindowRect;

typedef struct
{
    u32 id;
    WindowRect rect;
    const char* title;
    u32 app_id;
    bool visible;
    bool focused;
    bool dragging;
    s32 drag_offset_x;
    s32 drag_offset_y;
} Window;

void window_manager_init(void);
Window* window_manager_create_window(u32 app_id, const char* title, s32 x, s32 y, s32 w, s32 h);
void window_manager_handle_event(const Event* e);
void window_manager_render(Framebuffer* fb);
Window* window_manager_get_windows(u32* count);

#endif

