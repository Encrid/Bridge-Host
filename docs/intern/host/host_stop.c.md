# src/host/host_stop.c

This file unloads each loaded module at the end of a run, in the order of the pairs of `order.unload_before`.
Bridge states no order between modules, so the host gives it: `docs/extern/host/readme.md`, *The end of a run*.

## The steps

1. `pick_next` takes the loaded module that comes first by identity, among the modules that do not wait.
2. A module waits if `waits` finds a pair with its name second, and a loaded module first: `is_loaded`.
3. `host_unload_all` unloads that module with `bridge_unload`, which runs its `shutdown`. Then the slot of its file is no longer loaded.
4. The steps repeat until no module is loaded.

`is_named` matches a name of a pair with an identity: the name, then `@`. Thus a pair holds for each version of a module.
A pair whose module is not loaded makes no module wait. The pairs make no cycle, because `config_cycle` refuses a cycle.

A module that `bridge_unload` does not unload stays in the world, and `bridge_destroy` shuts it down after.
The steps make no allocation. They read the slots and the pairs again for each module, and the counts are small.

## Functions

| Function | Result |
| --- | --- |
| `host_unload_all` | Unloads each loaded module in its order. |
| `pick_next` | The next module to unload, or none. |
| `waits` | Whether a module waits for another loaded module. |
| `is_loaded` | Whether a module of a name is loaded. |
| `is_named` | Whether an identity has a name. |
