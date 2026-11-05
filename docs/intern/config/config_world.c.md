# src/config/config_world.c

This file takes the world keys: the phases, the keys of `WORLD_KEYS`, the budget and the order.

`take_phases` reads `phases` into an array of names.

`take_again` reads `again` into the caps of the phases, in the order of `phases`.
A file that states `again` with no phase, as `again = {}`, states the key. With no file that states it, `take_again` writes `not stated: again`.
A name of `again` that is not a name of `phases` gives its refusal of `readme.md`, *Messages*.

`take_rules` takes each rule of a table with a width: it finds the key over the files, and writes the value with `config_put`.
`config_world` takes `WORLD_KEYS` into the `deployment_t` of the host, and `BUDGET_KEYS` into its `bridge_budget_t`.
Then it takes the order through `config_order`.
A key that no file states makes `config_world` give false, after `config_find_key` wrote `not stated: <key>`.

## Functions

| Function | Result |
| --- | --- |
| `config_world` | Fills the world settings of the host. Gives true on success. |
| `take_again` | Fills the caps of the phases. Gives true on success. |
