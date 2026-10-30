# src/host_look.c

This file does one look: it walks the content, meets the changed files, and loads them.

## The look

`look` clears the seen marks, walks the content with `see_file`, then drops the files that are gone.
`see_file` finds the slot of a path first. A path with no slot is not read, and the look notes that the room is full.
Then it reads the file by its name in its folder, with `os_folder_read_file` of Bridge-OS.

The bytes belong to the folder, so `read_file` copies them when it meets a file whose hash mark changed.
It allocates one block for the `handed_t` and its bytes, so one `free` gives both back. An unchanged file costs no allocation.

`drop_gone` unloads a module whose file is gone, and frees the path and the `handed_t` of the file.
`order_files` sorts the found files by identity, then by path.

## A file that the system does not open

When Bridge-OS does not read a file, `see_file` writes `cannot read '<file>'`, and the work of the look grows by one.
The slot keeps the path with `keep_path`, and `unread` of the slot notes the failure, so the next look writes no line for it.
The slot stays seen, so a loaded module stays loaded. A read that works again clears `unread`.

## The full room

The room is `content.files` slots. `look_t` notes whether a path of this look found no slot, and `host_t` keeps that note of the last look.
`look` writes `too many files: <files> are looked at` when the room is full now and was not full at the last look.
Thus the line comes each time the room becomes full, and not at each round of a full room.
The line counts as work of the look, so the run does not end on that round.

## Functions

| Function | Result |
| --- | --- |
| `look` | Does one look and gives its work count, or -1 when a path fails. |
