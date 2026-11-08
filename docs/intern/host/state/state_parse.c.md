# src/host/state/state_parse.c

This file gives the function `parse` of the host to the library: the reader of a default of the manifest.

`state_parse` reads the text as the value of a TOML line `v = <text>`, with the reader of `src/host/toml`.
A text that is no value is kept as it is, as a text value. Thus `read=["game:Body"]` gives a list, and `name=Ada` gives the text `Ada`.
It gives `false` only when no memory is left.

## Functions

| Function | Result |
| --- | --- |
| `state_parse` | Fills a value from the text of a word. Gives false with no memory. |
