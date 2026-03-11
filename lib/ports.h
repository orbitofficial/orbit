#ifndef ORBIT_PORTS_H
#define ORBIT_PORTS_H

#include "types.h"

u8 inb(u16 port);
void outb(u16 port, u8 value);
u16 inw(u16 port);
void outw(u16 port, u16 value);

#endif

