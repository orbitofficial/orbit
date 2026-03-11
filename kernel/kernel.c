#include "kernel.h"
#include "../gui/framebuffer.h"
#include "../system/boot.h"
#include "../lib/types.h"

struct multiboot_tag
{
    u32 type;
    u32 size;
};

struct multiboot_tag_framebuffer
{
    u32 type;
    u32 size;
    u64 addr;
    u32 pitch;
    u32 width;
    u32 height;
    u8 bpp;
    u8 type_fb;
    u16 reserved;
};

static Framebuffer global_framebuffer;

void kmain(u32 magic, struct multiboot_tag* mb_info)
{
    if (magic != 0x36d76289)
    {
        while (1)
        {
        }
    }

    u32 addr = (u32)mb_info;
    addr += 8;

    Framebuffer fb;
    fb.width = 0;

    while (1)
    {
        struct multiboot_tag* tag = (struct multiboot_tag*)addr;
        if (tag->type == 0 && tag->size == 8)
        {
            break;
        }
        if (tag->type == 8)
        {
            struct multiboot_tag_framebuffer* fb_tag = (struct multiboot_tag_framebuffer*)tag;
            u32 width = fb_tag->width;
            u32 height = fb_tag->height;
            u32 pitch = fb_tag->pitch;
            u32 bpp = fb_tag->bpp;
            u8* fb_addr = (u8*)(u32)(fb_tag->addr & 0xFFFFFFFF);
            framebuffer_init(&fb, width, height, pitch, bpp, fb_addr);
            break;
        }
        addr += (tag->size + 7) & ~7;
    }

    if (fb.width == 0)
    {
        while (1)
        {
        }
    }

    global_framebuffer = fb;
    system_boot(&global_framebuffer);
    while (1)
    {
    }
}

