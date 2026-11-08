# src/manifest/manifest_keys.c

This file holds the keys of a module, in the table `KEYS`, in the order in which `manifest_state` takes them.
The 5 words of `docs/intern/manifest/manifest_take.c.md` come last, with `MANIFEST_CHECK_AUTHOR`: only the manifest states them.
The grants and the losses have no width, since they fill a list and its count.

`manifest_put` writes a value into the field of its key, at the offset and the width of the rule.
A flag gives `BRIDGE_TRUE` or `BRIDGE_FALSE`, and `on_error` gives `BRIDGE_FAIL` or `BRIDGE_SKIP`. The word `none` gives the value `none` of the rule.

## Functions

| Function | Result |
| --- | --- |
| `manifest_get_key` | The rule of a key, by its place, or null after the last. |
| `manifest_check_key` | The check of a value of a key, as the header gives it. |
| `manifest_put` | Writes a value into the config. |
