# src/host/config/config_value.c

This file checks a value against the rule of its key.

## The rules

A check of `manifest_check` takes its rule from `manifest_get_rule` of the library of the part `manifest`.
`RULES` holds one `manifest_rule_t` for each check that only the host has, from `CHECK_ARRIVE`: the allowed kinds, the bounds, the words, and the reason.
`config_check_value` gives the value and the rule to `manifest_check_value`, which gives the reason of the rule when the value is not allowed.

## Functions

| Function | Result |
| --- | --- |
| `config_check_value` | Gives null when a value fits its check, else the reason. |
