#ifndef ORBIT_MEMORY_H
#define ORBIT_MEMORY_H

#include "types.h"

void memory_set(void* dest, u8 value, u32 size);
void memory_copy(void* dest, const void* src, u32 size);

#endif

