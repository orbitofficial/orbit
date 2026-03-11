# Contributing to Orbit

Thank you for your interest in contributing to Orbit.

## Getting started

1. Fork the repository and clone your fork.
2. Set up the toolchain (on Debian/Ubuntu):

```bash
sudo apt update
sudo apt install build-essential gcc-multilib nasm grub-pc-bin xorriso qemu-system-x86
```

3. Build and run:

```bash
cd build
make
make run
```

## Development guidelines

- Keep each change focused and self‑contained.
- Preserve the existing directory boundaries:
  - `kernel` only for very low‑level boot and entry.
  - `core` for shared OS services (events, window manager).
  - `gui` for drawing and layout.
  - `drivers` for hardware access.
  - `system` to connect everything together.
- Do not add comments that simply restate obvious code behavior.
- Keep C and assembly free of comments unless they explain non‑obvious design decisions.
- Prefer small, composable functions over large monoliths.

## Coding style

- C and assembly use 4‑space indentation (see `.editorconfig`).
- No C99 variable‑length arrays, no dynamic allocation libraries.
- Use fixed‑width types from `lib/types.h` (`u32`, `s32`, and so on).

## Submitting changes

1. Create a feature branch in your fork.
2. Make your changes and ensure `make` succeeds.
3. Open a pull request:
   - Describe the motivation and behavior.
   - Mention any trade‑offs or limitations.

## Reporting issues

If you find a bug or have an idea:

- Open an issue describing:
  - What you expected to happen.
  - What actually happened.
  - Steps to reproduce.