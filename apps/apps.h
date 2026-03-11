#ifndef ORBIT_APPS_H
#define ORBIT_APPS_H

#include "../core/window_manager.h"
#include "../core/event.h"
#include "../gui/framebuffer.h"

void apps_init(void);
void apps_on_launch(u32 app_id);
void apps_render(Framebuffer* fb);
void apps_handle_event(const Event* e);

#endif

