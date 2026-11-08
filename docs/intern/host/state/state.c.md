# src/host/state/state.c

This file states a module with the library of the part `manifest`: `docs/extern/manifest/bridge_host_manifest.h.md`.

`get_module` fills a `manifest_host_t` with the functions of the state of the host, and its `ud` with the `stating_t`.
It gives the `manifest_module_t` of the module: the manifest and the bytes of the `handed_t`, and the `manifest_made_t` of the `made_t`.
`state_found` gives the module to `manifest_state`. `state_find_key` gives one key to `manifest_state_key`, for example `hot_reload` of a reload.

## Functions

| Function | Result |
| --- | --- |
| `get_module` | The module that the library states. |
| `state_found` | Fills a module config. Gives true when every key is taken. |
| `state_find_key` | What `manifest_state_key` gives. |
