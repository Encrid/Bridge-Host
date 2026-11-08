# src/host/state/state_phase.c

This file gives the function `phases` of the host to the library: the names of the phases that the deployment maps for a module.

`add_phase_names` lists the phase names under one table of the module.
`state_phases` gathers the names from the tables of the identity, of the backend and of `all`, each name one time.
The library finds the value of each name with `find`, and frees the block of the names.

## Functions

| Function | Result |
| --- | --- |
| `state_phases` | The count of the names, and their block. |
