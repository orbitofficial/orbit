#include "orbit.h"
#include "console.h"
#include "serial.h"
#include "vga.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "heap.h"
#include "fs.h"
#include "user.h"
#include "proc.h"
#include "api.h"
#include "app.h"
#include "builtins.h"
#include "net.h"
#include "udp.h"
#include "dhcp.h"
#include "rtl8139.h"
#include "netcmd.h"
#include "shell.h"
#include "log.h"
#include "splash.h"

static void warmup_serial(void)
{
    for (volatile int i = 0; i < 400000; i++)
        ;
    serial_putc('\n');
}

void kmain(void)
{
    serial_init();
    console_init();
    warmup_serial();

    gdt_init();
    idt_init();
    isr_install();
    pit_init(100);

    __asm__ volatile("sti");

    splash_show();
    console_clear();

    console_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
    console_printf("\n  %s\n", ORBIT_BANNER);
    console_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    console_printf("  CLI modular operating system  \n\n");

    console_printf("[boot] gdt\n");
    console_printf("[boot] idt + isr\n");
    console_printf("[boot] timer 100hz\n");
    console_printf("[boot] keyboard\n");
    keyboard_init();
    console_printf("[boot] heap\n");
    heap_init();

    console_printf("[boot] filesystem\n");
    fs_init();
    console_printf("[boot] users\n");
    user_init();
    console_printf("[boot] processes\n");
    proc_init();
    console_printf("[boot] application api\n");
    api_init();
    app_registry_init();
    builtins_register();

    console_printf("[boot] network\n");
    net_init();
    udp_init();
    dhcp_init();
    if (rtl8139_init() == 0)
        console_printf("[boot] rtl8139 nic detected\n");
    else
        console_printf("[boot] no network device\n");
    netcmd_register();

    log_init();

    klog("INFO", "Orbit %s boot complete", ORBIT_VERSION);
    console_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    console_printf("[boot] ready\n\n");
    console_set_color(VGA_LIGHT_GREY, VGA_BLACK);

    shell_run();

    for (;;)
        __asm__ volatile("hlt");
}
