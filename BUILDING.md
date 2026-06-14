# Building Orbit

This project builds a BIOS-bootable raw disk image for an x86_64 PC-style target. The kernel enters 64-bit long mode, uses VBE for graphics and is normally run with QEMU.

## Requirements

Install these tools:

```bash
gcc
ld
nasm
objcopy
make
python3
qemu-system-x86_64
```

## Build

```bash
make
```

This creates:

```text
build/kernel.elf
build/kernel.bin
build/boot.bin
build/orbit.img
```

The image layout is:

```text
LBA 0       boot sector
LBA 1..     kernel flat binary
LBA 8192    wallpaper binary asset
```

## Run

Graphical QEMU run:

```bash
make gui
```

Headless serial run:

```bash
make run
```

## Notes

The boot path is BIOS based, not UEFI based.

For VirtualBox: convert `build/orbit.img` to VDI first:

```bash
VBoxManage convertfromraw build/orbit.img build/orbit.vdi
```
