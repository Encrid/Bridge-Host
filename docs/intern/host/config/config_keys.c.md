# src/host/config/config_keys.c

This file defines the tables of the rules of the keys that the host reads, and the functions that use a rule.

## The tables

| Table | Keys | `at` is an offset into |
| --- | --- | --- |
| `WORLD_KEYS` | The keys of the world at the top of a file, the tables `checks` and `records`, and `files` and `depth` of `content`. | `deployment_t`. |
| `BUDGET_KEYS` | The keys of the table `budget`. | `bridge_budget_t`. |

A rule with a width of 0 is checked, and another source takes its value: `phases`.
Each world key and budget key is fixed, but the keys of `records` and of `content`.
`WORLD_KEY_COUNT` and `BUDGET_KEY_COUNT` give the length of each table.

## The functions

`config_get_rule` finds a rule by its name and its table. A key with no table matches a rule with no table.
Thus a check and a statement find a key in one place.
`config_get_names` writes the names of the key of a rule, one or two, and gives their count.

`config_put` writes a value at the offset of its rule, in the width of its field.
It writes `BRIDGE_TRUE` or `BRIDGE_FALSE` for a flag, and the `none` of the rule for the word `none`.

## Functions

| Function | Result |
| --- | --- |
| `config_get_rule` | Gives the rule of a key in a table, or null for a key that no rule has. |
| `config_get_names` | Writes the names of the key of a rule, and gives how many there are. |
| `config_put` | Writes a value into a struct, as its rule tells. |
