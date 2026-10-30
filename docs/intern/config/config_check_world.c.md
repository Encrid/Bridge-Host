# src/config/config_check_world.c

This file checks the world keys, the budget, the backends, and the content keys.

`check_rule` finds the rule of a key of one or two names in a table of rules, and checks the value of the key.
A key that is fixed draws the reason that it is fixed when the world is made, if the local file states it.
`config_check_world` checks `presets`, the budget with `BUDGET_KEYS`, and the other keys with `WORLD_KEYS`.
`config_check_backend` takes a key of three names: `library` is a text, and each other key is a setting of the backend.
`config_check_content` checks `paths`, `ignore`, a `load` flag, and an `arrive` word.

## Functions

| Function | Result |
| --- | --- |
| `config_check_world` | Checks a world key, or gives the reason it refuses. |
| `config_check_backend` | Checks a backend key. |
| `config_check_content` | Checks a content key. |
