# src/state/state_list.c

This file takes the list keys of a module: `read`, `write` and `losses`.

## The grants

`find_names` gathers a grant from the module tables, and stops at the first file that states it.
`permit_in` descends through a file and the files that it names, and tells whether one of them states the grant.
When no file states a grant, `state_take_list` takes the default of the manifest through `state_find_key`.
With no default, `state_find_key` writes `not stated: <key>`, and the module is not stated.
`say_names` prints a grant of the files when the host shows a module. `state_find_key` prints a default.

## The losses

`losses` takes its value through `state_find_key`, as a key with one value does.
A list gives the names, and `none` gives the count `BRIDGE_NONE`.

## The config

`put_names` writes the names and their count into the fields of the key in `bridge_module_config_t`.
The array of the names goes into the `made_t` of the module. The names point into the files or into the value of the manifest.

## Functions

| Function | Result |
| --- | --- |
| `state_take_list` | Takes a grant or the losses into the config. Gives true when a file or the manifest states the key. |
