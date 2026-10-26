# src/config/config_unfound.c

This file reports a module that a file names but that no look finds.

`get_named` reads an identity from a `module.<identity>` key or a `content.load.<identity>` key.
`is_found` tests whether a slot holds that identity.
`say_unfound` prints the refusal of an identity that no look finds, one time for each identity. The readme quotes the message.
`say_found_ones` then prints a `found:` line of the identities that the look did find.

## Functions

| Function | Result |
| --- | --- |
| `config_unfound` | Prints each named module with no match and gives the count. |
