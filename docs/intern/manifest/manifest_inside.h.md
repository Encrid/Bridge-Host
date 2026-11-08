# src/manifest/manifest_inside.h

This private header is shared by the sources of the library.

- `format_t` pairs a magic head with the function that locates the section of the manifest.
- `key_rule_t` gives a key of a module: its name and its check. Its width, its offset and `none` place its value in `bridge_module_config_t`.
- `key_path_t` gives the path of a key: its names and their count.
- `name_list_t` gives a list of names, each name one time.

## Functions

| Function | Result |
| --- | --- |
| `manifest_number` | Reads a little-endian number of a given width, with a bounds check. |
| `manifest_find_elf`, `manifest_find_macho`, `manifest_find_wasm` | Find the manifest section of one format. |
| `manifest_find` | Picks the format from the head and gives the backend name. |
| `manifest_take` | Takes the key, by its length, and the value of one `key=value` word into `manifest_t`. |
| `manifest_read_word` | Reads the identity or a later word, and gives the reason of a refused word. |
| `manifest_get_key` | The rule of a key, by its index. |
| `manifest_put` | Writes a value into the field of a key. |
| `manifest_keep`, `manifest_keep_value` | Keep a block or a value in the `manifest_made_t`. |
| `manifest_tell` | Gives one `manifest_told_t` to `tell` of the host. |
| `manifest_find_own` | A default of the manifest, after its check. |
| `manifest_own_words` | Tells the missing words of the manifest and the value of each word. |
| `manifest_find_value` | The value of a key, from the host or from the manifest. |
| `manifest_add_name` | Adds a name that a list does not hold yet. |
| `manifest_take_list` | Takes a grant or the losses. |
| `manifest_take_phases` | Takes the phases. |
| `manifest_take_settings` | Takes the settings. |
