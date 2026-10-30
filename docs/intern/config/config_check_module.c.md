# src/config/config_check_module.c

This file checks a key under `module`.

`check_table` accepts `all`, a backend name, or an identity with `:` and `@`, and turns down a bare version.
It frees the list of the backend names that it compares.
`check_rule` finds the rule of the third name with `config_get_rule`, and checks its value.
`check_setting` checks a key under `config`: a setting value, or a `path` text.
`config_check_module` refuses a key with too few names and routes the rest to these checks.

## Functions

| Function | Result |
| --- | --- |
| `config_check_module` | Gives null when a module key is right, else the reason. |
