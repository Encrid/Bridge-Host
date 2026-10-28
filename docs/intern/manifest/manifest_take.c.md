# src/manifest/manifest_take.c

This file takes one author key of a manifest into `manifest_t`.

`take_number` reads `abi` as an integer that is not negative.
`take_word` points `config` or `settings` at its value, in the bytes of the file, with no limit of length.
Each taker reads the key by its length with `is_key`, since the bytes keep the `=` in the word.
`take_flag` reads `pinned` or `reloadable` as `true` or `false`, and marks that the key was present.
`manifest_take` tries each taker in order.

## Functions

| Function | Result |
| --- | --- |
| `manifest_take` | Gives 1 when a key is taken, 0 when it is unknown, and -1 when the value is wrong. |
