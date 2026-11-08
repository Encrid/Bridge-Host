# src/manifest/manifest_own.c

This file reads the default of a key in the manifest.

1. `join_key` makes the key of the word from its path, for example `config.<setting>`.
2. `read_own` asks `manifest_get_value` how many words state the key. With 2 or more, it tells `MANIFEST_TOLD_TWICE`, and reads no value.
3. With 1 word, `parse` of the host reads the value.
4. `manifest_find_own` checks the value against the rule of the key. A wrong form tells `MANIFEST_TOLD_WRONG`, with the reason.

`manifest_tell` gives a `manifest_told_t` with no reason to `tell` of the host.

## Functions

| Function | Result |
| --- | --- |
| `join_key` | The key of a word, or null with no memory. |
| `read_own` | Reads the default of a key. Gives true when one word states it. |
| `manifest_find_own` | The checked default of a key, or null. |
| `manifest_tell` | Tells the host a value or a fault. |
