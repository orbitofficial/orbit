#ifndef ORBIT_STRING_H
#define ORBIT_STRING_H

#include "types.h"

u32 string_length(const char* s);
int string_compare(const char* a, const char* b);
void string_copy(char* dest, const char* src);

#endif

