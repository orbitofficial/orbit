#include "event.h"

#define EVENT_QUEUE_SIZE 128

static Event queue_buffer[EVENT_QUEUE_SIZE];
static u32 queue_head;
static u32 queue_tail;

void event_queue_init(void)
{
    queue_head = 0;
    queue_tail = 0;
}

bool event_queue_push(const Event* e)
{
    u32 next = (queue_tail + 1) % EVENT_QUEUE_SIZE;
    if (next == queue_head)
    {
        return false;
    }
    queue_buffer[queue_tail] = *e;
    queue_tail = next;
    return true;
}

bool event_queue_pop(Event* e)
{
    if (queue_head == queue_tail)
    {
        return false;
    }
    *e = queue_buffer[queue_head];
    queue_head = (queue_head + 1) % EVENT_QUEUE_SIZE;
    return true;
}

