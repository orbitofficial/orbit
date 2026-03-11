#include "framebuffer.h"

void framebuffer_init(Framebuffer* fb, u32 width, u32 height, u32 pitch, u32 bpp, u8* address)
{
    fb->width = width;
    fb->height = height;
    fb->pitch = pitch;
    fb->bpp = bpp;
    fb->address = address;
}

void framebuffer_clear(Framebuffer* fb, u32 color)
{
    for (u32 y = 0; y < fb->height; y++)
    {
        for (u32 x = 0; x < fb->width; x++)
        {
            framebuffer_put_pixel(fb, x, y, color);
        }
    }
}

void framebuffer_put_pixel(Framebuffer* fb, u32 x, u32 y, u32 color)
{
    if (x >= fb->width || y >= fb->height)
    {
        return;
    }
    u32 bytes_per_pixel = fb->bpp / 8;
    u32 offset = y * fb->pitch + x * bytes_per_pixel;
    u8* p = fb->address + offset;
    p[0] = (u8)(color & 0xFF);
    p[1] = (u8)((color >> 8) & 0xFF);
    p[2] = (u8)((color >> 16) & 0xFF);
    if (bytes_per_pixel == 4)
    {
        p[3] = (u8)((color >> 24) & 0xFF);
    }
}

