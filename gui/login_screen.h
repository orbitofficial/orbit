#ifndef ORBIT_LOGIN_SCREEN_H
#define ORBIT_LOGIN_SCREEN_H

#include "framebuffer.h"
#include "../core/event.h"

void login_screen_init(Framebuffer* fb);
void login_screen_render(Framebuffer* fb);
bool login_screen_handle_event(const Event* e);

#endif

