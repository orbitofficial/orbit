<div align="center" style="display:grid;place-items:center;">
<p>
    <a href="https://orbit.org/" target="_blank"><img width="700" src="https://socialify.git.ci/orbit/orbit/image?description=1&font=Inter&forks=1&issues=1&logo=https%3A%2F%2Fgithub.com%2Forbit%2Forbit%2Fblob%2Fmaster%2F.github%2Flogo.png%3Fraw%3Dtrue&name=1&owner=1&pattern=Plus&pulls=1&stargazers=1&theme=Auto" alt="The Orbit logo"></a>
</p>
</div>

Orbit is a modern graphical operating system, architected for scale and versatility. With a modular core and advanced desktop environment, Orbit sets a professional foundation for research, education, and deployment in real-world scenarios.

## Project layout

- `kernel` – entry point, multiboot2 setup
- `core` – core services such as event queue and window manager
- `gui` – framebuffer, rendering, desktop, boot and login screens
- `drivers` – low level input drivers (mouse, keyboard)
- `system` – system bootstrap and main loop
- `lib` – shared utility code (types, memory, strings, ports)
- `build` – Makefile, GRUB config, linker script

## Building

Orbit is currently built and run from Linux using the system `gcc`, `ld`, and GRUB tools.

### Dependencies (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential gcc-multilib nasm grub-pc-bin xorriso qemu-system-x86
```

### Build and run

```bash
cd build
make          # builds kernel.elf and orbit.iso
make run      # runs qemu-system-i386 -cdrom orbit.iso
```

## Contribution

We welcome to contributions! You can visit contribution guidelines from [here](./CONTRIBUTING.MD).

## License

[GNU General Public License](./LICENSE)