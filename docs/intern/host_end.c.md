# src/host_end.c

This file writes the lines of the end of a run, before Bridge destroys the world.

## The lists of Bridge

`say_loaded`, `say_profile` and `say_usage` each ask Bridge for the count of the rows of a list first.
Then each one allocates the rows, and asks Bridge to fill them. Thus a list has no fixed size.
If no memory is left for the rows, the list writes no row.

| Function | Bridge gives | The host writes |
| --- | --- | --- |
| `say_loaded` | `bridge_list_modules` | `still loaded:`, and each module, or `nothing`. |
| `say_profile` | `bridge_profile` | `profile <name>: <calls> calls, <nanos> ns`, for each system and observer. |
| `say_usage` | `bridge_usage` and `bridge_module_usage` | A line for each rule of `BUDGET_KEYS`, then a line for each loaded module. |

`get_field` reads a field of a `bridge_budget_t` at the offset and in the width of its rule.
Thus the lines of the usage name each key of the budget as the deployment does.

## The end

`say_end` writes the modules that stay loaded.
It writes the profile when `profile` of the world is `BRIDGE_TRUE`, and the usage when `usage` is `BRIDGE_TRUE`.
The host reads no clock: each number comes from Bridge.

## Functions

| Function | Result |
| --- | --- |
| `say_end` | Writes the lines of the end of a run. |
