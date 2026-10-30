# src/config/config_keys.c

This file defines `MODULE_KEYS`, the table of keys that the host reads for a module.

Each `module_rule_t` names a key, its check, the width of its field, the offset into `bridge_module_config_t`, and the value for `none`.
The author keys `abi`, `config`, `settings`, `pinned` and `reloadable` carry the `CHECK_AUTHOR` mark.
`MODULE_KEY_COUNT` reports the length of the table.
`config_get_rule` finds the rule of a key by its name. Thus a check and a statement find a key in one place.

## Functions

| Function | Result |
| --- | --- |
| `config_get_rule` | Gives the rule of a key of a module, or null for a name that no rule has. |
