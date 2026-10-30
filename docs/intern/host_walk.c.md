# src/host_walk.c

This file walks the content paths and offers each file to a callback.

## The walk

`walk_content` steps over every path of `content.paths`, with a `walk_t` that holds the host and the callback.
It opens a path as a folder with `os_folder_open` of Bridge-OS, and gives the folder to `walk_folder`.
A path that is no folder goes to `walk_file`.

`walk_folder` reads the entries of a folder with `os_folder_read_entry`, and skips the ignored ones.
It offers each file to the callback, with the open folder and the name of the file.
It opens a folder in it only if more than one level is left, so `content.depth` bounds the walk.
A folder in it that does not open is left out. `is_ignored` tests a path against `content.ignore` by its whole form.

`walk_file` opens the folder of a content path that names one file: the part up to its last separator, that `config_folder_size` finds.
A path with no separator opens `.`, the folder of the deployment.
It reads the file one time, and offers it to the callback only if the read gives its bytes.
Else it prints `cannot look in '<path>'`, and the walk stops. The callback reads the file again.

## The paths

`join_path` joins the path of the folder and the name of an entry in fresh memory, at the length of the two.
It puts a separator between them, but not after a path that ends with a separator.
The host opens a folder by this path, and reads each file of the folder by its name. Thus a file can be at each depth.
Bridge-OS opens a long path of a folder in parts, as the document of its `os.h` tells.
If no memory is left for a path, the walk goes on to the next entry.

## Functions

| Function | Result |
| --- | --- |
| `walk_content` | Offers each content file to the callback. Gives 0, or -1 when a path fails. |
