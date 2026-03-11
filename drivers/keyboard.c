#include "keyboard.h"
#include "../lib/ports.h"
#include "../core/event.h"

static unsigned char last_scancode;

void keyboard_init(void)
{
    last_scancode = 0;
}

void keyboard_poll(void)
{
    u8 status = inb(0x64);
    if (!(status & 0x01) || (status & 0x20))
    {
        return;
    }
    u8 scancode = inb(0x60);
    if (scancode == last_scancode)
    {
        return;
    }
    last_scancode = scancode;
    Event e;
    e.type = EVENT_KEY_DOWN;
    e.x = 0;
    e.y = 0;
    e.button = 0;
    e.pressed = 1;
    e.key = scancode;
    e.app_id = 0;
    event_queue_push(&e);
}

