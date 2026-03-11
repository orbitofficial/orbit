#include "memory.h"

void memory_set(void* dest, u8 value, u32 size)
{
    u8* d = (u8*)dest;
    for (u32 i = 0; i < size; i++)
    {
        d[i] = value;
    }
}

void memory_copy(void* dest, const void* src, u32 size)
{
    u8* d = (u8*)dest;
    const u8* s = (const u8*)src;
    for (u32 i = 0; i < size; i++)
    {
        d[i] = s[i];
    }
}

