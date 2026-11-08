# src/manifest/manifest_made.c

This file keeps the memory that the library allocates while it states one module, in the `manifest_made_t` of the module.
The config points into it, so it stays until `manifest_free`.

With no memory for the list, `manifest_keep` and `manifest_keep_value` free what they got, and give null.
`manifest_free_value` also frees the items of each item.

## Functions

| Function | Result |
| --- | --- |
| `add_one` | Adds a pointer to a list. Gives false with no memory. |
| `manifest_keep` | The kept block, or null. |
| `manifest_keep_value` | The kept value, or null. |
| `manifest_free_value` | Frees what a value holds. |
| `manifest_free` | Frees each block, each value, and the lists. |
