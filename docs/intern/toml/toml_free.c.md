# src/toml/toml_free.c

This file frees what the TOML reader allocated.

`toml_free` frees the array of the key and the value of each entry, then the entries.
Then it frees each name, and the list of the names.
`toml_free_value` frees the text of a value, and each item of a list, then the items.

## Functions

| Function | Result |
| --- | --- |
| `toml_free` | Frees the entries and the names of a `toml_t`. |
| `toml_free_value` | Frees the text and the items of a value. |
