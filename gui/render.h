#ifndef ORBIT_RENDER_H
#define ORBIT_RENDER_H

#include "framebuffer.h"
#include "../lib/types.h"

void render_rect(Framebuffer* fb, u32 x, u32 y, u32 w, u32 h, u32 color);
void render_char(Framebuffer* fb, u32 x, u32 y, char c, u32 fg, u32 bg);
void render_text(Framebuffer* fb, u32 x, u32 y, const char* text, u32 fg, u32 bg);

#endif

