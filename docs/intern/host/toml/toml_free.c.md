# src/host/toml/toml_free.c

This file frees what the TOML reader allocated.

`toml_free` frees the array of the key and the value of each entry, then the entries.
Then it frees each name, and the list of the names.
It frees a value with `manifest_free_value` of the library, since a `value_t` is a `manifest_value_t`.

## Functions

| Function | Result |
| --- | --- |
| `toml_free` | Frees the entries and the names of a `toml_t`. |
