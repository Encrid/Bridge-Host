# src/manifest/manifest_list.c

This file takes the list keys of a module: `read`, `write` and `losses`.

`manifest_take_list` finds the value with `manifest_find_value`. A list gives its names, each name one time.
For `losses`, the word `none` gives the count `BRIDGE_NONE`.
`put_names` writes the names and their count into the fields of the key. The block of the names goes into the `manifest_made_t`.

## Functions

| Function | Result |
| --- | --- |
| `manifest_add_name` | Adds a name to a list, if the list does not hold it. |
| `put_names` | Writes a list into the config. |
| `manifest_take_list` | Gives true when the host or the manifest states the key. |
