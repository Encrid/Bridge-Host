# src/state/state_own.c

This file reads the author values from the manifest and reports the author keys.

## A default of the manifest

`read_own` asks `manifest_get_value` how many words state the key.
With two words or more, it writes `its file states <key> twice` after the prefix of the statement, and reads no value.
With one word, `parse_own` reads the value as a TOML value, or keeps it as a text.
`parse_own` takes the value out of the parsed `toml_t`, then frees the `toml_t`.
A value that is no TOML value keeps the memory of the line that `parse_own` gave to the reader.
`state_find_own` checks the value against the rule of the key, and keeps it in the `made_t` of the module.

## The author keys

`state_own_keys` demands `config`, `pinned` and `reloadable`, and prints a missing one.
It reads the mask `taken` of the manifest for `pinned` and `reloadable`.
`say_own_keys` prints the author keys when the host shows a module.

## Functions

| Function | Result |
| --- | --- |
| `read_own` | Reads a manifest value into a `value_t`. Gives true when one word states the key. |
| `state_find_own` | Gives the checked author value of a key, or null. |
| `state_own_keys` | Gives true when the manifest states the needed author keys. |
