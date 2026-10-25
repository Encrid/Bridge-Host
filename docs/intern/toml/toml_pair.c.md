# src/toml/toml_pair.c

This file reads a statement and an inline table.

`toml_read_pair` reads the key, and `read_assigned` requires an `=`, and then reads a value or an inline table.
`read_assigned` frees a value that `toml_add` refuses.

An inline table is a brace that carries members which commas separate.
`read_member` handles one member: it copies the outer key, adds the member name, and stores the value.
A member can itself be an inline table, so `toml_read_inline` nests.
`toml_read_pair` and `read_member` free the array of their key at the end. The names stay in the `toml_t`.

## Functions

| Function | Result |
| --- | --- |
| `toml_read_pair` | Reads one `key = value` statement, or refuses an absent `=`. |
| `toml_read_inline` | Reads a brace of members, or refuses an absent `}`. |
