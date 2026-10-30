# src/manifest/manifest_take.c

This file takes one author word of a manifest into `manifest_t`.

## The author words

`NAMES` holds the keys `abi`, `config`, `settings`, `pinned` and `reloadable`, and `WORDS` holds the mark of each one.
`manifest_take` finds the key in `NAMES` by its length with `is_key`, since the bytes keep the `=` in the word.
A key that is not there is unknown, and the host does not read it.

`manifest_take` refuses a key whose mark is in `taken` already. Thus a manifest cannot state an author word twice.
Then it sets the mark, and `take_value` reads the value:

- `abi` is an integer that is not negative.
- `config` and `settings` point at their value, in the bytes of the file, with no limit of length.
- `take_flag` reads `pinned` or `reloadable` as `true` or `false`.

## Functions

| Function | Result |
| --- | --- |
| `manifest_take` | Gives 1 for a key that it takes, 0 for an unknown key, -1 for a wrong value, and -2 for a key taken before. |
