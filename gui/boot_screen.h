#ifndef ORBIT_BOOT_SCREEN_H
#define ORBIT_BOOT_SCREEN_H

#include "framebuffer.h"
#include "../core/event.h"

void boot_screen_init(Framebuffer* fb);
void boot_screen_render(Framebuffer* fb);
bool boot_screen_handle_event(const Event* e);

#endif

