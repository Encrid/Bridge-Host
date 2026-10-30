# src/host_walk.c

This file walks the content paths and offers each file to a callback.

## The walk

`walk_content` steps over every path of `content.paths`, with a `walk_t` that holds the host and the callback.
`walk_path` offers a lone file, or reads a folder, and prints `cannot look in '<path>'` when the path is neither.

`walk_folder` reads the entries of a folder, and skips the ignored ones. It offers each file to the callback.
It reads a folder in it only if more than one level is left, so `content.depth` bounds the walk.
`is_ignored` tests a path against `content.ignore` by its whole form. An ignored folder is not read.

## The paths

`join_path` joins the path of the folder and the name of an entry in fresh memory, at the length of the two.
Thus a path has no limit of length. If the system refuses to open a long path, the host does not read that file.
`join_path` gives null for the entries `.` and `..`, so the walk does not join them, and does not open them.
If no memory is left for a path, the walk goes on to the next entry.

## Functions

| Function | Result |
| --- | --- |
| `walk_content` | Offers each content file to the callback. Gives 0, or -1 when a path fails. |
