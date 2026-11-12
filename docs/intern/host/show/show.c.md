# src/host/show/show.c

This file prints a configuration for `--show-config`.

## The output

`say_files` prints a line per file, with the file that named it and a mark for the local file.
`say_world` prints each key of `WORLD_KEYS` and `BUDGET_KEYS`, the keys of `again`, the table of each backend, the content words, and the paths.
It prints `again = {}` if a file states it so, and the cap of each phase that `again` names. `say_key` prints a key that no file states as nothing.
`say_key` and `say_under` print one key, or every key below a prefix of 1 or 2 names.

`show_config` prints the files, then the world, then the pairs, then walks each content path once with `walk_path` and `show_module`.
It prints each pair of `order.before` as `order: <a> before <b>`, and each pair of `order.unload_before` as `unload: <a> before <b>`.

## Functions

| Function | Result |
| --- | --- |
| `show_config` | Shows a configuration. Gives 0, or 1 on a refusal. |
