#ifndef ORBIT_EVENT_H
#define ORBIT_EVENT_H

#include "../lib/types.h"

typedef enum
{
    EVENT_NONE = 0,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_BUTTON,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,
    EVENT_APP_LAUNCH
} EventType;

typedef struct
{
    EventType type;
    s32 x;
    s32 y;
    u8 button;
    u8 pressed;
    u8 key;
    u32 app_id;
} Event;

void event_queue_init(void);
bool event_queue_push(const Event* e);
bool event_queue_pop(Event* e);

#endif

