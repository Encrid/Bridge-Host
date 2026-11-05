# src/config/config_again.c

This file reads the key `again` of the world into the caps of the phases, in the order of `phases`.

`config_again` takes each name under `again` with `config_names`, and the cap of each one with `take_cap`.
A file that states `again` with no phase, as `again = {}`, states the key.
With no file that states it, `config_again` writes `not stated: again`.
`take_cap` finds the phase of a name with `get_phase`.
A name that is not a name of `phases` gives its refusal of `readme.md`, *Messages*.
Two files that state one cap with 2 values give the refusal of `config_say_found`.

## Functions

| Function | Result |
| --- | --- |
| `config_again` | Fills the caps of the phases. Gives true on success. |
| `take_cap` | Takes the cap of one name. Gives true on success. |
| `get_phase` | Gives the place of a name in `phases`, or `phase_count`. |
