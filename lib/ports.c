#include "ports.h"

u8 inb(u16 port)
{
    u8 value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

void outb(u16 port, u8 value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "dN"(port));
}

u16 inw(u16 port)
{
    u16 value;
    __asm__ volatile("inw %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

void outw(u16 port, u16 value)
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "dN"(port));
}

