# src/manifest/manifest_state.c

This file states a module into a `bridge_module_config_t`: `docs/extern/manifest/bridge_host_manifest.h.md`, *A module*.

`manifest_find_value` asks `find` of the host first. With 0, it takes the default of the manifest, and tells its value, or `MANIFEST_TOLD_NOT_STATED`.
With -1, the host told the fault, and the key has no value.
`take_key` skips a key of `MANIFEST_CHECK_AUTHOR`, gives a key with no width to `manifest_take_list`, and puts each other value into its field.
`get_flag` maps a boolean onto `BRIDGE_TRUE` or `BRIDGE_FALSE`.

## Functions

| Function | Result |
| --- | --- |
| `get_flag` | The flag of a boolean. |
| `manifest_find_value` | The value of a key, or null. |
| `take_key` | Takes one key into the config. Gives true on success. |
| `manifest_state` | Fills a module config. Gives true when every key is taken. |
| `manifest_state_key` | The value of one key, or null. |
