# src/config/config_keys.c

This file defines the tables of the rules of the keys that the host reads, and the functions that use a rule.

## The tables

| Table | Keys | `at` is an offset into |
| --- | --- | --- |
| `MODULE_KEYS` | The keys of a module. | `bridge_module_config_t`. |
| `WORLD_KEYS` | The keys of the world at the top of a file, and the table `checks`. | `deployment_t`. |
| `BUDGET_KEYS` | The keys of the table `budget`. | `bridge_budget_t`. |

The author keys `abi`, `config`, `settings`, `pinned` and `reloadable` carry the `CHECK_AUTHOR` mark.
A rule with a width of 0 is checked, and another source takes its value: a list of a module, or `phases`.
Each world key and budget key is fixed. Each module key can be in the local file.
`MODULE_KEY_COUNT`, `WORLD_KEY_COUNT` and `BUDGET_KEY_COUNT` give the length of each table.

## The functions

`config_get_rule` finds a rule by its name and its table. A key with no table matches a rule with no table.
Thus a check and a statement find a key in one place.
`config_get_names` writes the names of the key of a rule, one or two, and gives their count.

`config_put` writes a value at the offset of its rule, in the width of its field.
It writes `BRIDGE_TRUE` or `BRIDGE_FALSE` for a flag, the item of `enum bridge_on_error` for `on_error`, and the `none` of the rule for the word `none`.
`config_get_setting` makes a `bridge_setting_t` of a value: an integer, a real, a flag, or a text that the caller gives.

## Functions

| Function | Result |
| --- | --- |
| `config_get_rule` | Gives the rule of a key in a table, or null for a key that no rule has. |
| `config_get_names` | Writes the names of the key of a rule, and gives how many there are. |
| `config_put` | Writes a value into a struct, as its rule tells. |
| `config_get_setting` | Gives the setting of a key and its value. |
