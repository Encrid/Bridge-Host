# src/config/config_name.c

This file reads the files that `presets` names, and guards against a loop.

`config_take_file` reuses a file by its whole path, or adds it and then reads its presets.
`refuse_loop` prints `refused: <file> names itself:` with the chain when a file leads back to itself.
`config_take_file` owns the path: it frees the path of a file that it has already.
`take_named` reads the `presets` list and follows each file in order.
`add_named` records that one file names another. `is_paths` checks that `presets` is a list of texts.

## Functions

| Function | Result |
| --- | --- |
| `config_take_file` | Adds a file with its presets and gives its index, or -1 on a problem. |
