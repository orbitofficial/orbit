#ifndef ORBIT_FRAMEBUFFER_H
#define ORBIT_FRAMEBUFFER_H

#include "../lib/types.h"

typedef struct Framebuffer
{
    u32 width;
    u32 height;
    u32 pitch;
    u32 bpp;
    u8* address;
} Framebuffer;

void framebuffer_init(Framebuffer* fb, u32 width, u32 height, u32 pitch, u32 bpp, u8* address);
void framebuffer_clear(Framebuffer* fb, u32 color);
void framebuffer_put_pixel(Framebuffer* fb, u32 x, u32 y, u32 color);

#endif

