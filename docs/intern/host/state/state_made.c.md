# src/host/state/state_made.c

This file keeps the memory that the host allocates while it states one module.

The config of a module points into this memory, and Bridge reads the config during `bridge_load` or `bridge_reload`.
Thus the memory stays until the call returns. Then `state_free` frees it with `manifest_free`.
`state_keep` keeps a block in the `manifest_made_t` of the `made_t`, with `manifest_keep` of the library.
Thus 1 list holds the blocks of the host and of the library.
When the block cannot be kept, `manifest_keep` frees it, and `state_keep` prints `no memory left to keep a block`.

`state_keep_prefix` makes the prefix of the lines of a statement, at the length of its parts, and adds it to the `made_t`.
A prefix is `<what>: <path>: <identity>: `. If no memory is left, the prefix is empty, and the statement goes on.

## Functions

| Function | Result |
| --- | --- |
| `state_keep_prefix` | Gives the prefix of the lines of a statement, or an empty text. |
| `state_keep` | Keeps a block in the `manifest_made_t`, and gives the block, or null with its line. |
| `state_free` | Frees the `manifest_made_t`: each block of the host and of the library. |
