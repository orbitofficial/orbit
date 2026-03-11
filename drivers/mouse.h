#ifndef ORBIT_MOUSE_H
#define ORBIT_MOUSE_H

#include "../lib/types.h"

void mouse_init(void);
void mouse_poll(void);
void mouse_set_bounds(s32 max_x, s32 max_y);
s32 mouse_get_x(void);
s32 mouse_get_y(void);
u8 mouse_get_buttons(void);

#endif

