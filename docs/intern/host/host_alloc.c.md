# src/host/host_alloc.c

This file gives the blocks of memory of part host, and states each block that the system refuses.

## The rule

Each allocation of part host calls `host_malloc`, `host_calloc` or `host_realloc`, in place of the function of the C library.
When the system refuses a block, the function writes `no memory left for <bytes> bytes`, and gives null.
The caller then gives its own failure, as before. Thus no refused block ends the host with no line, as the need Explicit of `docs/contributing/needs.md` asks.

`host_calloc` gives the product of its count and its size as the bytes. When the product passes `SIZE_MAX`, `ckd_mul` tells it, and the line gives `SIZE_MAX`.
A block of 0 bytes that is refused writes no line, since the C library can give null for 0 bytes with no failure.

The toml reader of `src/host/toml` and `src/host/say.c` keep the functions of the C library.
The toml reader writes its own line, `refused: <file>:<line>: no memory left`. `say` writes the line of this file, so it cannot call it.

## Functions

| Function | Result |
| --- | --- |
| `said` | Writes the line of a refused block, and gives the block. |
| `host_malloc` | A block of `bytes`, or null with the line. |
| `host_calloc` | A block of `count` items of `size` bytes, each byte 0, or null with the line. |
| `host_realloc` | The block grown or shrunk to `bytes`, or null with the line. The old block stays then. |
