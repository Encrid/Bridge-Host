# src/say.h

This header declares the writers of the lines of the host.
A `say_line_t` holds the line that the host builds: its text and the count of its bytes.
The attribute `gnu::format` lets the compiler check each format and its arguments, as for `printf`.

## Functions

| Function | Result |
| --- | --- |
| `say_part` | Adds a part to the line that the host builds. |
| `say` | Adds the last part of the line, then gives the whole line to the output. |
