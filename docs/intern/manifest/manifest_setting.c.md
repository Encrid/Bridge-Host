# src/manifest/manifest_setting.c

This file takes the settings that a module declares.

1. `split_names` splits the word `settings` of the manifest into names, each name one time.
2. `undeclared` of the host tells each setting of a file that is not one of these names.
3. A module with `config=none` and settings tells `MANIFEST_TOLD_NO_COMPONENT`.
4. `take_setting` finds the value of each setting with `manifest_find_value`, from the host or from the word `config.<name>`.

`manifest_get_setting` makes a `bridge_setting_t`: `docs/extern/manifest/bridge_host_manifest.h.md`, *The values*.
A setting of the form `{ path = "..." }` takes the text that the host gives in `text` of `manifest_found_t`.

## Functions

| Function | Result |
| --- | --- |
| `manifest_get_setting` | The setting of a value. |
| `take_setting` | Takes one setting. Gives true on success. |
| `split_names` | The names of the settings. |
| `manifest_take_settings` | Fills the settings. Gives true when every setting is taken. |
