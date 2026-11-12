# src/host/host.h

This private header gathers the whole host.
It sets the `arrive` choices, the outcomes of a replacement, the kinds of a look, and the structs that one run shares.
It then declares the functions that the other sources of `src` define.

## The data

`host_t` holds one run: the parsed `config_t`, the `deployment_t`, the backends, the content paths and the `bridge_world_t`.
It also holds the array of the `file_t` slots, the found files of a look, and a watch for each content path.
Of the run, it keeps the count `round` of the rounds that ran, and `ended`: whether a module asked for the end.
`again` holds the cap of each phase, in the order of `phases`. A phase that the key `again` does not name has 0.
It also keeps `full`: whether the last look found a path with no free slot.

`orders` holds 2 `pairs_t`, by `enum order_kind`: the pairs of systems of `order.before` for Bridge, and the pairs of modules of `order.unload_before`.
A `pair_asked_t` asks `config_states_pair` for one pair under one of the 2 keys of `ORDER_KEYS`.

A `file_t` records one known file by its path, its hash mark, the module behind it, and in `at` the content path that holds it.
Its `unread` notes that the last read of the file failed.
A `handed_t` keeps the bytes of a file together with the manifest that the host read from them.
A `held_t` names a backend, its library path, its start rule and its arrive rule, and the array of its settings.

`stating_t`, `look_t`, `seeing_t` and `walk_t` carry state while the host states a module or walks the content.
A `made_t` lists what the host allocates for the config of one module, with the `manifest_made_t` of the library. A `stating_t` points at it.

## What a deployment does not choose

The host fixes these parts. A deployment cannot state them, for the reason that the table gives.

| Part | Value | Reason |
| --- | --- | --- |
| The output | The host writes its lines to the standard output. | It is no number and no flag. |
| The end that a module or a signal asks for | The run ends after the round that runs. | It is no number and no flag. |
| The answer that loads a module | `yes` | It is no number and no flag. |
| The names of the overlay and of the lock | `<file>.d/local.toml` and `<file>.d/lock` | They are no number and no flag. |
| The order of the loads | By identity, then by path. | The host has one implementation of it. |
| The pace of the rounds | A round starts when the round before it ends. | The host reads no clock. |

## Functions

| Function | Result |
| --- | --- |
| `config_world`, `config_again`, `config_order`, `config_backends`, `config_content` | Take the world, the caps of `again`, the order, the backend and the content keys. |
| `config_states_pair`, `config_cycle` | Find a pair in a file, and refuse a cycle of the pairs of `order.unload_before`. |
| `config_load`, `config_unfound`, `get_held` | Read `content.load`, report named modules that no look finds, and find a backend. |
| `run`, `run_phase`, `say_end`, `look` | Run the world, run one phase as often as it asks and its cap allows, write the lines of its end, and do one look. |
| `host_unload_all` | Unload the modules that stay at the end, by the pairs. |
| `walk_path`, `walk_name`, `is_ignored`, `join_path` | Walk one content path, read one file, test an ignored path, and join a path. |
| `see_file`, `read_path`, `look_path`, `retry_unread`, `stop_watches` | Read one file, read a path whole, look at one path, read again the files that failed, and stop the watches. |
| `find_file`, `keep_path`, `drop_gone` | Find the slot of a path, keep a path in its slot, and free the slots of the files that are gone. |
| `meet`, `may_arrive`, `load_found`, `make_artifact` | Read a met file, ask whether it may load, load it, and build its artifact. |
| `show_config`, `show_module`, `take_lock` | Show a configuration, show one module, and take the run lock. |
| `state_found`, `state_find_key`, `state_parse`, `state_find`, `state_find_grant`, `state_phases`, `state_undeclared`, `state_tell` | State a module with the library, and give the library the keys of the deployment. |
| `state_is_setting`, `state_states_setting`, `state_is_path` | Find a setting in a file. |
| `state_keep_prefix`, `state_keep`, `state_free` | Make the prefix of a statement, add a block to the `made_t` of a module, and free it. |
| `host_free` | Free the host. |
