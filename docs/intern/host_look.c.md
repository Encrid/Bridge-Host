# src/host_look.c

This file does one look: it walks the content, meets the changed files, and loads them.

## The look

`look` clears the seen marks, walks the content with `see_file`, then drops the files that are gone.
`see_file` finds the slot of a path first. A path with no slot is not read, and the look notes that the room is full.
`read_file` reads a file that has a slot, and meets it when the hash mark changes.
It allocates a `handed_t` only for a file that it meets. An unchanged file costs no allocation but its bytes.

`drop_gone` unloads a module whose file is gone, and frees the path and the `handed_t` of the file.
`order_files` sorts the found files by identity, then by path.

## The full room

`look_t` notes whether a path of this look found no slot, and `host_t` keeps that note of the last look.
`look` writes `too many files: 32 are looked at` when the room is full now and was not full at the last look.
Thus the line comes each time the room becomes full, and not at each round of a full room.
The line counts as work of the look, so the run does not end on that round.

## Functions

| Function | Result |
| --- | --- |
| `look` | Does one look and gives its work count, or -1 when a path fails. |
