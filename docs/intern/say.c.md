# src/say.c

This file builds each line of the host, and gives the whole line to the output.

## The lines

`say_part` and `say` give each part of a line to `add`, which keeps the line in a `static` `say_line_t` of its own.
`add` measures each part with `vsnprintf`, and grows the line with `realloc` to that length. So a line can have each length.
After the last part, which `say` gives, `emit` gives the whole line to the C library with one `fwrite`, and frees it.
Then `say` calls `fflush` for the standard output. Thus the system gets each line of the host in one piece, when its end comes.

If no memory is left, `add` gives the line so far and the part to the output at once.
Thus no text is lost, but the system can get that line in more than one piece.

The standard output keeps the buffer that the C library gives it, also when it is a pipe or a file.
A native module can write into the same output.
Its lines and the lines of the host then come in the order that they were written.
Bridge-OS writes the line of the first signal past that buffer, with one call of the system.
As each line of the host goes out at its end, the line of a signal comes between two lines.

## Functions

| Function | Result |
| --- | --- |
| `say_part` | Adds a part to the line. |
| `say` | Adds the last part of the line, and writes the line. |
