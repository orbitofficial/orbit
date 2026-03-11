#ifndef ORBIT_DESKTOP_H
#define ORBIT_DESKTOP_H

#include "framebuffer.h"
#include "render.h"
#include "../core/event.h"

typedef struct
{
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} Rect;

typedef struct
{
    Rect area;
    const char* title;
    u32 app_id;
} DesktopIcon;

void desktop_init(Framebuffer* fb);
void desktop_render(Framebuffer* fb);
bool desktop_handle_event(const Event* e);

Rect desktop_get_taskbar_rect(Framebuffer* fb);
Rect desktop_get_icon_rect(u32 index);
u32 desktop_get_icon_count(void);
const DesktopIcon* desktop_get_icons(void);

#endif

