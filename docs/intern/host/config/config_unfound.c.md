# src/host/config/config_unfound.c

This file reports a module that a file names but that no look finds.

`get_named` reads an identity from a `module.<identity>` key or a `content.load.<identity>` key.
`is_found` tests whether a slot holds that identity.
`say_unfound` prints the refusal of an identity that no look finds, one time for each identity. The readme quotes the message.
With no memory for the list of the said identities, it prints the refusal each time, so no refusal is lost.
`say_found_ones` then prints a `found:` line of the identities that the look did find.

## Functions

| Function | Result |
| --- | --- |
| `config_unfound` | Prints each named module with no match and gives the count. |
