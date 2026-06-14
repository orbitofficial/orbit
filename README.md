# Orbit

A modular operating system written from scratch, featuring a 64-bit x86_64 architecture and a graphical user interface (GUI).

## Features

<details>
<summary>Bootloader</summary>

* Custom bootloader written in NASM
* Loads the kernel using INT 13h LBA
* Sets VBE 1024x768x32 video mode with 800x600x32 fallback
* Enables A20
* Sets up the GDT
* Switches from protected mode to long mode

</details>

<details>
<summary>Kernel</summary>

* 64-bit C kernel
* Freestanding
* Flat binary
* Loaded at 0x10000
* 4 GB identity mapping using 2 MB pages
* GDT / IDT / ISR / IRQ / PIC / PIT interrupt infrastructure for 64-bit mode
* Heap support with kmalloc / kfree

</details>

<details>
<summary>Graphics</summary>

* Double buffered compositor running on the VBE linear framebuffer
* Single copy framebuffer flips when pitch allows it

</details>

<details>
<summary>Desktop Environment</summary>

* Wallpaper
* Taskbar
* Start menu
* Clock

</details>

<details>
<summary>Window Manager</summary>

* Draggable windows
* Focus handling
* Shadows

</details>

<details>
<summary>Input, Serial and Disk</summary>

* PS/2 mouse and keyboard support
* Serial input support, allowing headless operation
* ATA PIO disk reads for loading the wallpaper asset from the disk image

</details>

<details>
<summary>Builtin Applications and OS Services</summary>

* Terminal shell
* Files
* System Monitor
* About
* Users

</details>

## Memory Map

| Address | Contents |
|-------|--------|
| `0x01000`–`0x07000` | Page tables: PML4 / PDPT / PD |
| `0x07C00` | Bootloader |
| `0x08000` | VBE mode information |
| `0x10000` | Kernel: text / data / bss |
| `0x400000`–`0x4400000` | Heap (64 MB) |
| LBA `8192` | Wallpaper binary asset |
| VBE LFB | Framebuffer (usually `0xFD000000`) |

## Building and Running

See [BUILDING.md](BUILDING.md).
