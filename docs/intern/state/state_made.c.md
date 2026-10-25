# src/state/state_made.c

This file keeps the memory that the host allocates while it states one module.

The config of a module points into this memory, and Bridge reads the config during `bridge_load` or `bridge_reload`.
Thus the memory stays until the call returns. Then `state_free` frees it.
`state_keep` adds a block to the `made_t` of a `stating_t`, and `state_keep_value` adds a value.
If no memory is left for the list, each one frees what it was given, and gives null.

## Functions

| Function | Result |
| --- | --- |
| `state_keep` | Adds a block to the list, and gives the block, or null. |
| `state_keep_value` | Adds a value to the list, and gives the value, or null. |
| `state_free` | Frees each block, each value with its texts and items, and the list. |
