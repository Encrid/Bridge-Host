# src/config/config_free.c

This file frees a configuration.

`config_free_file` frees the path, the folder, the whole path, the `toml_t` and the named files of one file.
`config_free` frees each file, then the array of the files.

## Functions

| Function | Result |
| --- | --- |
| `config_free` | Frees each file of a configuration, and the array of the files. |
| `config_free_file` | Frees what one file owns. |
