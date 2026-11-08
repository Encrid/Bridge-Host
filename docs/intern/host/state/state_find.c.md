# src/host/state/state_find.c

This file gives the function `find` of the host to the library: a key of a module, as the deployment states it.

`state_find` picks the search of the key:

1. `config.<setting>` goes to `find_setting`, which finds `module."<identity>".config.<setting>` in the files.
2. A grant, `read` or `write`, goes first to `state_find_grant` of `state_list.c`.
3. Each other key goes to `find_key`, which finds it in the 3 tables of the module: `docs/extern/host/readme.md`, *The modules*.

Each search gives 0 when no file states the key, and the library then takes the default of the manifest.
A clash of 2 files gives its lines with `config_say_found`, then -1. Else the search gives 1 and the value.
When the host shows a module, the search writes the value, its file and its line with `config_say_value`.
A setting of the form `{ path = "..." }` gives in `text` its path from the folder of its file, kept in the `made_t`.

## Functions

| Function | Result |
| --- | --- |
| `find_key` | 1 and the value in the tables of the module, 0, or -1. |
| `find_setting` | 1 and the value of a setting of the module, 0, or -1. |
| `state_find` | The result of the search of the key. |
