# src/show/show.c

This file prints a configuration for `--show-config`.

## The output

`say_files` prints a line per file, with the file that named it and a mark for the local file.
`say_world` prints each key of `WORLD_KEYS` and `BUDGET_KEYS`, the keys of `again`, the table of each backend, the content words, the paths, and the order.
`say_again` prints `again` with no phase as `again = {}`, and each phase that it names with its cap.
`say_key` and `say_under` print one key, or every key below a prefix.
`show_config` prints the files, then the world, then walks each content path once with `walk_path` and `show_module`.

## Functions

| Function | Result |
| --- | --- |
| `show_config` | Shows a configuration. Gives 0, or 1 on a refusal. |
