# src/host/host_path.c

This file joins the path of an entry, and tells whether the host ignores a path.

`join_path` joins the path of a folder and the name of an entry in fresh memory, at the length of the two.
It puts a separator between them, but not after a path that ends with a separator.
The host opens a folder by this path, and reads each file of the folder by its name. Thus a file can be at each depth.
mosl opens a long path of a folder in parts, as the document of its `os.h` tells.
If no memory is left for a path, a walk goes on to the next entry, and a take reads the whole folder.

`is_ignored` tests a path against `content.ignore` by its whole form, from `os_path_whole` of mosl.
A path is ignored if an ignored path is equal to it, or holds it.

## Functions

| Function | Result |
| --- | --- |
| `is_ignored` | Tells whether `content.ignore` holds a path. |
| `join_path` | Gives the path of an entry of a folder, which the caller frees, or null. |
