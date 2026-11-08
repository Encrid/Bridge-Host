# src/manifest/manifest_phase.c

This file takes the phase map of a module.

`manifest_take_phases` gets the names of the phases from `phases` of the host. `take_phase` finds the value of each one with `find`, and pairs the name with it.
A phase has no default in the manifest, so a name with no value tells `MANIFEST_TOLD_NOT_STATED`.
The phase map goes into the `manifest_made_t`. `manifest_take_phases` frees the block of the names.

## Functions

| Function | Result |
| --- | --- |
| `take_phase` | Pairs a phase with its value. Gives true on success. |
| `manifest_take_phases` | Fills the phase map. Gives true when every phase resolves. |
