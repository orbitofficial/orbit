#ifndef ORBIT_KERNEL_H
#define ORBIT_KERNEL_H

#include "../lib/types.h"

struct multiboot_tag;

void kmain(u32 magic, struct multiboot_tag* mb_info);

#endif

