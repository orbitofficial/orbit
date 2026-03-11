TARGET=orbit
ISO_DIR=iso
BOOT_DIR=$(ISO_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub
KERNEL_BIN=kernel.elf

CC=gcc
AS=nasm
LD=ld

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -I../kernel -I../core -I../gui -I../drivers -I../lib -I../system
LDFLAGS=-T linker.ld -nostdlib -z max-page-size=0x1000 -m elf_i386

KERNEL_SRC=$(wildcard ../kernel/*.c) $(wildcard ../core/*.c) $(wildcard ../gui/*.c) $(wildcard ../drivers/*.c) $(wildcard ../lib/*.c) $(wildcard ../system/*.c)
KERNEL_ASM=$(wildcard ../kernel/*.s)

KERNEL_OBJ=$(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.s=.o)

all: $(TARGET).iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) -f elf32 $< -o $@

$(KERNEL_BIN): $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(KERNEL_OBJ)

$(TARGET).iso: $(KERNEL_BIN) grub.cfg
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL_BIN) $(BOOT_DIR)/
	cp grub.cfg $(GRUB_DIR)/
	grub-mkrescue -o $(TARGET).iso $(ISO_DIR)

run: $(TARGET).iso
	qemu-system-i386 -cdrom $(TARGET).iso

clean:
	rm -f $(KERNEL_OBJ) $(KERNEL_BIN)
	rm -rf $(ISO_DIR)

.PHONY: all run clean

