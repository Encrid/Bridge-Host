# src/host/host_load.c

This file loads a found module into the world.

## The load

`load_found` runs the gates, applies the start rule or the arrive rule of the backend, then calls `bridge_load`.
It keeps the memory of the statement in a `made_t`, and frees it when `bridge_load` returns.
`is_left` turns a module down when a peer already holds its name, or when the host lacks its backend.
Bridge refuses a module of another ABI in `bridge_load`, from the `abi` that the manifest states, so the host has no check of its own.

`is_stated` reads `content.load` and builds the artifact when the module is to load.
Its prefix `not loaded: <file>: <identity>: ` comes from `state_keep_prefix`, at the length of its parts.
`make_artifact` fills a `bridge_artifact_t` with the bytes and the module config. For the backend `native`, it also gives the path.
Thus BB-Native copies the same bytes that gave the manifest, under the name of the file.
`get_same` finds a loaded module of the same name.

## Functions

| Function | Result |
| --- | --- |
| `load_found` | Loads a module and prints the outcome. |
| `make_artifact` | Builds the artifact of a module. Gives true when its keys are taken. |
