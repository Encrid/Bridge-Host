# src/toml/toml_value.c

This file reads a value and branches on its first character.

`toml_read_value` empties the value, then reads a text, a list, a flag, or a number, and refuses a brace or an empty value.
After a refusal, the value can hold a part of a list, and the caller frees it.

`read_flag` accepts `true` or `false`.
`read_list` reads items until a `]`, and demands a `,` or a `]` after each one.
`read_list` frees an item that it does not add.
`skip_space` passes the spaces, comments and newlines within a list. `add_item` extends the list by one value.

## Functions

| Function | Result |
| --- | --- |
| `toml_read_value` | Reads one value, or refuses a value that is none. |
