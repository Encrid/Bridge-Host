# src/host_walk.c

This file walks a content path, and offers each file to a callback.

## The walk

`walk_path` opens a content path as a folder with `os_folder_open` of mosl, and gives the folder to `walk_folder`.
A path that is no folder goes to `walk_file`.

`walk_folder` reads the entries of a folder with `os_folder_read_entry`.
It skips the ignored ones, with `is_ignored` of `src/host_path.c`.
It offers each file to the callback, with the open folder and the name of the file.
It opens a folder in it only if more than one level is left, so `content.depth` bounds the walk.
A folder in it that does not open is left out.

## One file

`walk_name` reads one file by its whole path. It opens the folder of the file: the part up to its last separator, that `config_folder_size` finds.
A path with no separator opens `.`, the folder of the deployment.

It reads the file one time, and offers it to the callback only if the read gives its bytes. The callback reads the file again.
`walk_file` offers a content path that names one file. If `walk_name` cannot read it, `walk_file` prints `cannot look in '<path>'`, and the look stops.

## Functions

| Function | Result |
| --- | --- |
| `walk_path` | Offers each file of one content path to the callback. Gives 1 for a folder, 0 for a file, or -1 when the path fails. |
| `walk_name` | Offers one file, by its whole path, to the callback. Gives true when the file reads. |
