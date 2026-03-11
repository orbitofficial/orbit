#include "mouse.h"
#include "../lib/ports.h"
#include "../core/event.h"

static s32 mouse_x;
static s32 mouse_y;
static u8 mouse_buttons;
static u8 packet[3];
static u8 packet_index;
static s32 mouse_max_x = 1023;
static s32 mouse_max_y = 767;
static s32 mouse_sensitivity = 2;

static void mouse_wait_write(void)
{
    while (inb(0x64) & 0x02)
    {
    }
}

static void mouse_wait_read(void)
{
    while (!(inb(0x64) & 0x01))
    {
    }
}

static void mouse_write(u8 value)
{
    mouse_wait_write();
    outb(0x64, 0xD4);
    mouse_wait_write();
    outb(0x60, value);
}

static u8 mouse_read(void)
{
    mouse_wait_read();
    return inb(0x60);
}

void mouse_init(void)
{
    u8 status = inb(0x64);
    outb(0x64, 0xA8);
    mouse_wait_write();
    outb(0x64, 0x20);
    mouse_wait_read();
    status = inb(0x60) | 2;
    mouse_wait_write();
    outb(0x64, 0x60);
    mouse_wait_write();
    outb(0x60, status);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
    mouse_x = 320;
    mouse_y = 240;
    mouse_buttons = 0;
    packet_index = 0;
    mouse_max_x = 1023;
    mouse_max_y = 767;
    mouse_sensitivity = 2;
}

void mouse_set_bounds(s32 max_x, s32 max_y)
{
    mouse_max_x = max_x;
    mouse_max_y = max_y;
}

void mouse_poll(void)
{
    u8 status = inb(0x64);
    if (!(status & 0x01) || !(status & 0x20))
    {
        return;
    }
    u8 data = inb(0x60);
    packet[packet_index++] = data;
    if (packet_index < 3)
    {
        return;
    }
    packet_index = 0;
    u8 flags = packet[0];
    s8 dx = (s8)packet[1];
    s8 dy = (s8)packet[2];
    s32 old_x = mouse_x;
    s32 old_y = mouse_y;
    mouse_x += dx * mouse_sensitivity;
    mouse_y -= dy * mouse_sensitivity;
    if (mouse_x < 0)
    {
        mouse_x = 0;
    }
    if (mouse_y < 0)
    {
        mouse_y = 0;
    }
    if (mouse_x > mouse_max_x)
    {
        mouse_x = mouse_max_x;
    }
    if (mouse_y > mouse_max_y)
    {
        mouse_y = mouse_max_y;
    }
    u8 new_buttons = flags & 0x07;
    if (new_buttons != mouse_buttons)
    {
        u8 changed = new_buttons ^ mouse_buttons;
        for (u8 i = 0; i < 3; i++)
        {
            if (changed & (1 << i))
            {
                Event e;
                e.type = EVENT_MOUSE_BUTTON;
                e.x = mouse_x;
                e.y = mouse_y;
                e.button = i;
                e.pressed = (new_buttons & (1 << i)) ? 1 : 0;
                e.key = 0;
                e.app_id = 0;
                event_queue_push(&e);
            }
        }
        mouse_buttons = new_buttons;
    }
    if (mouse_x != old_x || mouse_y != old_y)
    {
        Event move;
        move.type = EVENT_MOUSE_MOVE;
        move.x = mouse_x;
        move.y = mouse_y;
        move.button = 0;
        move.pressed = 0;
        move.key = 0;
        move.app_id = 0;
        event_queue_push(&move);
    }
}

s32 mouse_get_x(void)
{
    return mouse_x;
}

s32 mouse_get_y(void)
{
    return mouse_y;
}

u8 mouse_get_buttons(void)
{
    return mouse_buttons;
}

