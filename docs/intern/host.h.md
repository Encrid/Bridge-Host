# src/host.h

This private header gathers the whole host.
It sets the `arrive` choices, the outcomes of a replacement, and the structs that one run shares.
It then declares the functions that the other sources of `src` define.

## The data

`host_t` holds one run: the parsed `config_t`, the `deployment_t`, the backends, the content paths and the `bridge_world_t`.
It also holds the array of the `file_t` slots, and the found files of a look.
Of the run, it keeps the count `round` of the rounds that ran, and `ended`: whether a module asked for the end.
It also keeps `full`: whether the last look found a path with no free slot.

A `file_t` records one known file by its path, its hash mark, and the module behind it.
A `handed_t` keeps the bytes of a file together with the manifest that the host read from them.
A `held_t` names a backend, its library path, its start rule and its arrive rule, and the array of its settings.

`stating_t`, `look_t`, `seeing_t` and `walk_t` carry state while the host states a module or walks the content.
A `made_t` lists what the host allocates for the config of one module, and a `stating_t` points at it.

## What a deployment does not choose

The host fixes these parts. A deployment cannot state them, for the reason that the table gives.

| Part | Value | Reason |
| --- | --- | --- |
| The output | The host writes its lines to the standard output. | It is no number and no flag. |
| The end that a module or a signal asks for | The run ends after the round that runs. | It is no number and no flag. |
| The word of trust | `yes` | It is no number and no flag. |
| The names of the overlay and of the lock | `<file>.d/local.toml` and `<file>.d/lock` | They are no number and no flag. |
| The order of the loads | By identity, then by path. | The host has one implementation of it. |
| The pace of the rounds | A round starts when the round before it ends. | The host reads no clock. |

## Functions

| Function | Result |
| --- | --- |
| `config_world`, `config_order`, `config_backends`, `config_content` | Take the world, order, backend and content keys. |
| `config_load`, `config_unfound`, `get_held` | Read `content.load`, report named modules that no look finds, and find a backend. |
| `run`, `say_end`, `look`, `walk_content` | Run the world, write the lines of its end, do one look, and walk the content paths. |
| `find_file`, `meet`, `may_arrive`, `load_found`, `make_artifact` | Track a slot, read a met file, ask to trust it, load it, and build its artifact. |
| `show_config`, `show_module`, `take_lock` | Show a configuration, show one module, and take the run lock. |
| `state_*`, `read_own` | State the keys, lists, phases and settings of a module. |
| `state_keep_prefix`, `state_keep`, `state_keep_value`, `state_free` | Make the prefix of a statement, add a block or a value to the `made_t` of a module, and free the list. |
| `host_free`, `handed_free` | Free the host, and free one `handed_t`. |
