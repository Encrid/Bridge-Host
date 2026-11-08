# src/host/host_look.c

This file does one look: it reads the content paths, meets the changed files, and loads them.

## The look

`look` gives each content path to `look_path` of `src/host/host_watch.c`, with the kind of the look:

| Kind | When | What |
| --- | --- | --- |
| `LOOK_FIRST` | At the first round. | It starts a watch of each folder, then reads each path whole. |
| `LOOK_NOTICES` | At each later round. | It reads the files that a watch names, and a path whole only when it must. |
| `LOOK_ALL` | At the end of a run with `live = false`. | It reads each path whole. |

Then `look` reads again each file that did not read, with `retry_unread`.
It writes the line of a full room, sorts the found files, and loads them.

`read_path` reads one path whole. It first clears the seen marks of the files of that path.
Then it walks the path with `see_file`, and `drop_gone` drops the files of that path that the walk did not see.
Thus a file of another path is not taken for gone.

`see_file` finds the slot of a path first. A path with no slot is not read, and the look notes that the room is full.
Then it reads the file by its name in its folder, with `os_folder_read_file` of mosl.
It notes in `at` of the slot the content path that holds the file.

The bytes belong to the folder, so `read_file` copies them when it meets a file whose hash mark changed.
It allocates one block for the `handed_t` and its bytes, so one `free` gives both back. An unchanged file costs no allocation.
`order_files` sorts the found files by identity, then by path.

## A file that the system does not open

When mosl does not read a file, `see_file` writes `cannot read '<file>'`, and the work of the look grows by one.
The slot keeps the path with `keep_path`, and `unread` of the slot notes the failure, so the next look writes no line for it.
The slot stays seen, so a loaded module stays loaded. A read that works again clears `unread`.

## The full room

The room is `content.files` slots. `look_t` notes whether a path of this look found no slot, and `host_t` keeps that note.

`look` writes `too many files: <files> are looked at` when the room is full now and was not full before.
Thus the line comes each time the room becomes full, and not at each round of a full room.
A look that reads each path whole replaces the note of `host_t`. A look that reads only named files can only set it, because it does not see each file.
The line counts as work of the look, so the run does not end on that round.

## Functions

| Function | Result |
| --- | --- |
| `see_file` | Reads one file that a walk offers, and meets it. |
| `read_path` | Reads one content path whole, and gives what `walk_path` gives. |
| `look` | Does one look of a kind, and gives its work count, or -1 when a path fails. |
