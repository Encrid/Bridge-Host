# src/host_watch.c

This file looks at one content path. It takes the changes that the system tells, and reads the path whole when it must.

## The watch

Each content path has a slot in `watches` of `host_t`: the watch of Bridge-OS on that folder, or null.
At the first look, `look_path` starts a watch of the path with `os_watch_start`, at `content.depth`.
Then it reads the path whole. The watch starts first, so a change during that read is told after it.
A path that is a file gets no watch.

At a later look, `take_names` takes the names of the watch with `os_watch_take` until none is left.
It joins each name to the path, skips an ignored one, and reads the file with `walk_name`.
If a named entry does not read as a file, the look reads the whole path.
Thus the look finds files that were removed or moved in, and files that another program holds.
If the watch lost changes or stopped, `look_path` stops it, starts a new watch, and reads the whole path.

A path with no watch is read whole at each look.
For a folder that gets no watch at the first look or at a new start, `look_path` gives the line `cannot watch '<path>': read at each round`.
The line comes only for a folder, because a file never gets a watch.

`retry_unread` reads again each file whose last read failed, so a file that reads again is found when the system names nothing.
`stop_watches` stops each watch at the end of the run, and frees the slots.

## Functions

| Function | Result |
| --- | --- |
| `look_path` | Looks at one content path, as *The watch* tells. Gives what `read_path` gives, 0 when it reads nothing whole, or -1 when the path fails. |
| `retry_unread` | Reads again each file whose last read failed. |
| `stop_watches` | Stops each watch, and frees the slots of the watches. |
