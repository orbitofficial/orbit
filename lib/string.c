#include "string.h"

u32 string_length(const char* s)
{
    u32 len = 0;
    while (s[len] != 0)
    {
        len++;
    }
    return len;
}

int string_compare(const char* a, const char* b)
{
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return (int)((unsigned char)*a) - (int)((unsigned char)*b);
}

void string_copy(char* dest, const char* src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = 0;
}

