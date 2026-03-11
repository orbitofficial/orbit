# Agents and AI usage in Orbit

This repository is designed to work well with AI coding assistants.

## Goals

- Keep the project structure predictable so agents can navigate it quickly.
- Keep configuration, build steps, and conventions explicit in text files.
- Avoid generated or binary artifacts in version control where possible.

## Conventions for agents

- Prefer small, focused changes per edit.
- Keep architecture and layering intact:
  - `kernel` only does very low‑level boot and entry.
  - `core` holds shared services (events, window manager).
  - `gui` owns all drawing and layout.
  - `drivers` own direct hardware access.
  - `system` wires the above together.
- Do not add comments that simply restate obvious code.
- Preserve the no‑comments rule inside C and assembly code that already follows it.

## When extending Orbit

- Add new subsystems as separate directories where possible (for example `net`, `fs`, `pkg`).
- Keep new public interfaces in headers under the appropriate directory.
- Update `README.md` and this file when major architectural changes are introduced so future agents can understand the layout and intent quickly.