# src/host/host_list.c

This file lists the entries of 1 folder in the order of their names.
Thus the walk does not depend on the order of the file system.

## The list

`list_folder` reads each entry of the folder with `os_folder_read_entry` of mosl.
mosl gives the entries in the order of the file system, and that order differs by system and by file system: https://github.com/Encrid/mosl/blob/v0.6.1/docs/extern/os.h.md, *Functions*.
`list_folder` copies each name, because a name of mosl stays valid only until the next read.
Then it sorts the entries with `qsort` and `strcmp`, so the order is the order of the bytes of the names, on each system.

When a block for the list or for a name is refused, `list_folder` gives false.
Then the list holds the entries that it took until then, and `free_listing` frees them, as in each other case.

## Functions

| Function | Result |
| --- | --- |
| `order_listed` | Compares 2 entries by the bytes of their names. |
| `add_listed` | Adds a copy of 1 entry to the list. Gives false when a block is refused. |
| `list_folder` | Lists each entry of a folder, sorted by name. Gives false when no memory is left. |
| `free_listing` | Frees the names and the list, and empties the list. |
