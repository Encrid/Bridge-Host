# src/host/host_path.c

This file joins the path of an entry, and tells whether the host ignores a path.

`join_path` joins the path of a folder and the name of an entry in fresh memory, at the length of the two.
It puts a separator between them, but not after a path that ends with a separator.
The host opens a folder by this path, and reads each file of the folder by its name. Thus a file can be at each depth.
mosl opens a long path of a folder in parts, as the document of its `os.h` tells.

If no memory is left for a path, a walk stops: `docs/intern/host/host_walk.c.md`. A take then reads the whole folder, so it loses no change.

`is_ignored` tests a path against `content.ignore` by its whole form, from `os_path_whole` of mosl.
A path is ignored if an ignored path is equal to it, or holds it.
For an ignored path, it writes `ignored: <path>` as a record of the level `BRIDGE_DEBUG`, with the rule of `[records]`.
Both paths of a file test it: the walk of a folder (`src/host/host_walk.c`), and a name that a watch tells (`src/host/host_watch.c`).
Thus the case of `tests/cases/host/look.toml` with the ignored name holds on each system. It waits for that line, and both paths write it, also when the watch loses the notice and the host reads the whole folder.

## Functions

| Function | Result |
| --- | --- |
| `is_ignored` | Tells whether `content.ignore` holds a path, and writes the line of an ignored path. |
| `join_path` | Gives the path of an entry of a folder, which the caller frees, or null. |
