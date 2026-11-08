# include/manifest/bridge_host_manifest.h

This header gives the manifest of a module that runs on Bridge, and the rules that make a `bridge_module_config_t` from it.
A host links `manifest.a`, and gives the keys of a module through the functions of a `manifest_host_t`.

## The manifest

`manifest_read` finds the manifest in the bytes of a file, and reads its words into a `manifest_t`.
The form of the file gives the backend: `native` for a library, and `wasm` for a module of wasm.
The words, their places in each format and their checks are in `docs/extern/host/readme.md`, *The manifest of a module*.

| Field of `manifest_t` | Value |
| --- | --- |
| `identity` | The first word: `<namespace>:<name>@<version>`. |
| `backend` | The name of the backend of the form of the file. |
| `config` | The value of the word `config`. |
| `settings` | The value of the word `settings`, or an empty text. |
| `abi` | The value of the word `abi`. |
| `pinned`, `reloadable` | The values of the words `pinned` and `reloadable`. |
| `taken` | The bits of `manifest_word` of each of these 5 words that the manifest states. |

| Bit of `manifest_word` | Word |
| --- | --- |
| `MANIFEST_ABI` | `abi` |
| `MANIFEST_CONFIG` | `config` |
| `MANIFEST_SETTINGS` | `settings` |
| `MANIFEST_PINNED` | `pinned` |
| `MANIFEST_RELOADABLE` | `reloadable` |

The texts point into the bytes of the file, so the bytes must stay while the host uses the manifest.
`manifest_read` gives no text after a manifest that it took. Else it gives the reason of the refusal, for example `no manifest`.

`manifest_get_value` gives the count of the words of a key, for example `scratch`, and the value of the first one.

## The values

`manifest_value_t`, the type of `struct manifest_value`, holds a value of a key. Its `kind` tells which field holds it:

| `kind` | Field |
| --- | --- |
| `MANIFEST_TEXT` | `text` |
| `MANIFEST_WHOLE` | `whole` |
| `MANIFEST_REAL` | `real` |
| `MANIFEST_FLAG` | `flag` |
| `MANIFEST_LIST` | `count` items in `items`. |
| `MANIFEST_TABLE` | `count` items in `items`, each with its name in `text`. |

`manifest_free_value` frees the texts and the items of a value, and not the value itself.

`manifest_rule_t` gives the forms that a key allows, with the bits of `manifest_allow` in its field `allow`:

| Bit | Allowed value |
| --- | --- |
| `MANIFEST_ALLOW_FLAG` | `true` or `false`. |
| `MANIFEST_ALLOW_WHOLE` | A whole number, from `least` to `most`. |
| `MANIFEST_ALLOW_REAL` | A real number. |
| `MANIFEST_ALLOW_WORD` | One of the words of `words`, each followed by a space. |
| `MANIFEST_ALLOW_TEXT` | A text that is not empty. |
| `MANIFEST_ALLOW_EMPTY` | With `MANIFEST_ALLOW_TEXT`, also an empty text. |
| `MANIFEST_ALLOW_NAMES` | A list, each of whose items is a text with at least 1 byte. |
| `MANIFEST_ALLOW_PAIRS` | A list, not empty, of lists of 2 such texts. |

`manifest_check_value` gives no text for a value that the rule allows. Else it gives `why` of the rule.
`manifest_check_key` checks the value of a key of a module, as `docs/extern/host/readme.md`, *The modules*, gives each key.
For a name that is no key of a module, it gives `no key of a module`. For a key that only the manifest states, for example `abi`, it gives `only a module's author states it, in its file`.

`manifest_get_setting` makes the `bridge_setting_t` of a value: a whole number, a real number or a flag by its kind, and `text` for each other value.
The host gives as `text` the text of the value, or the whole path of a setting of the form `{ path = "..." }`.

`manifest_get_rule` gives the rule of a check of the keys of a module. A host can check its own keys with these rules too.

| `manifest_check` | `why` of the rule |
| --- | --- |
| `MANIFEST_CHECK_FLAG` | `true or false` |
| `MANIFEST_CHECK_COUNT` | `a whole number, from 1` |
| `MANIFEST_CHECK_LIMIT` | `a whole number, from 1, or "none"` |
| `MANIFEST_CHECK_LEVEL` | `a level, from 1 to 7` |
| `MANIFEST_CHECK_NAMES` | `a list of names` |
| `MANIFEST_CHECK_NAMES_NONE` | `a list of names, or "none"` |
| `MANIFEST_CHECK_ERROR` | `"fail" or "skip"` |
| `MANIFEST_CHECK_TEXT` | `text` |
| `MANIFEST_CHECK_SETTING` | `text, a number, true or false, or { path = "..." }` |
| `MANIFEST_CHECK_AUTHOR` | `only a module's author states it, in its file` |

## The host

`manifest_host_t` holds the functions that the host gives, and `ud`, which each function gets first.

| Function | Work |
| --- | --- |
| `parse` | Reads the text of a default of the manifest as a value of the host's own reader. A text that is no value is a text value. It gives `false` only with no memory. |
| `find` | Gives the value of a key of the module, from the keys of the host. `names` is the path of the key: `{ "scratch" }`, `{ "phases", "<phase>" }` or `{ "config", "<setting>" }`. |
| `phases` | Gives the names of the phases that the host maps for the module, in a block that `malloc` gave. The library frees the block. |
| `undeclared` | Gets the names of the settings of the manifest, and tells each setting that the host states and that is not one of them. It gives their count. |
| `tell` | Gets each value that the manifest gives, and each fault, as a `manifest_told_t`. |

`find` gives 1 with the value, or 0 when the host states no value. It gives -1 after a fault that the host told itself, for example a clash.
For a setting stated as a path, `text` of `manifest_found_t` gives the path that the host made whole. Else `text` is no text.
For `read` and `write`, the host gives the whole list, also when it adds up the lists of some files.

`names` and `depth` give the path of the key. `value` and `why` hold a value and a reason, when the kind has them.

| `kind` of `manifest_told_t` | When |
| --- | --- |
| `MANIFEST_TOLD_VALUE` | The manifest gives the value of `names`: a default, or `config`, `settings`, `pinned` or `reloadable`. |
| `MANIFEST_TOLD_NOT_STATED` | Neither the host nor the manifest gives a value of `names`. |
| `MANIFEST_TOLD_TWICE` | The manifest states the default of `names` twice. |
| `MANIFEST_TOLD_WRONG` | The default of `names` has a form that its rule does not allow. `why` gives the reason. |
| `MANIFEST_TOLD_NO_WORD` | The manifest does not state the word `names`: `config`, `pinned` or `reloadable`. |
| `MANIFEST_TOLD_NO_COMPONENT` | The manifest names settings, and `config=none`. |

The texts of a `manifest_told_t` live only during the call of `tell`.

## A module

`manifest_module_t` holds what the library reads to state one module: `host`, the manifest `who`, `bytes` and `len` of the file, and `made`.
`manifest_state` fills a `bridge_module_config_t` with each key of the module, in this order:

1. The identity, the ABI of Bridge, `config`, `pinned` and `reloadable` come from the manifest.
2. Each key of `docs/extern/host/readme.md`, *The modules*, comes from `find`, else from the default of the manifest.
3. The phases come from `phases` and `find`.
4. The settings come from `find`, else from the defaults of the manifest. `undeclared` comes before them.

It gives `true` when it took each key. After a fault it goes on with the next key, so `tell` gets each fault of the module.
`manifest_state_key` gives the value of one key, from `find` or from the manifest, or no value after a fault.

The config points into the memory of `manifest_made_t`, and Bridge reads it during `bridge_load` or `bridge_reload`.
After that call, `manifest_free` frees that memory: the `block_count` blocks of `blocks`, and the `value_count` values of `values`.
A `manifest_made_t` starts with each field at 0.

## Functions

| Function | Result |
| --- | --- |
| `manifest_read` | No text, or the reason why the bytes hold no manifest that the library takes. |
| `manifest_get_value` | The count of the words of a key, and the value of the first one. |
| `manifest_check_value` | No text, or the reason why a rule does not allow a value. |
| `manifest_check_key` | No text, or the reason why a key of a module does not allow a value. |
| `manifest_get_rule` | The rule of a check. |
| `manifest_get_setting` | The setting of a value. |
| `manifest_state` | `true` when the config holds each key of the module. |
| `manifest_state_key` | The value of one key of a module, or no value. |
| `manifest_free` | Frees the memory of the states of a module. |
| `manifest_free_value` | Frees what a value holds. |
