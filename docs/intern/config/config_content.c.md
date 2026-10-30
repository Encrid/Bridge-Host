# src/config/config_content.c

This file gathers the content paths and reads `content.load`.

`collect` reads `content.paths` or `content.ignore` from every file and merges the lists.
`add_path` drops a path whose whole form is already present.
`add_path` frees each path that it does not keep.
`config_content` fills the path and ignore arrays, and prints `not stated: content.paths` when none exist.
Then it allocates the slots of the files and the list of the found files, `content.files` of each.
`config_load` reads `content.load.<identity>`, and falls back to `content.load.all`.

## Functions

| Function | Result |
| --- | --- |
| `config_content` | Fills the content paths and allocates the slots. Gives true when a path exists and the memory is there. |
| `config_load` | Gives 1 to load, 0 to leave, or -1 on a problem. |
