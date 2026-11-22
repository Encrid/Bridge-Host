# src/host/host_alloc.h

This header gives `host_malloc`, `host_calloc` and `host_realloc` of `src/host/host_alloc.c` to part host.
`src/host/host.h` and `src/host/config/config.h` include it, so each file of part host can call them.

## Functions

| Function | Source | Result |
| --- | --- | --- |
| `host_malloc`, `host_calloc`, `host_realloc` | `src/host/host_alloc.c` | A block of memory, or null with a line: `docs/intern/host/host_alloc.c.md`, *The rule*. |
