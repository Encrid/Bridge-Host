# src/host/state/state_list.c

This file finds the grants of a module in the deployment: `read` and `write`.

`find_names` gathers a grant from the module tables, and stops at the first file that states it.
`permit_in` descends through a file and the files that it names, and tells whether one of them states the grant.
`say_names` prints a grant of the files when the host shows a module.
`make_list` makes a list value of the names, in the `made_t` of the module. The names point into the files.
When no file states a grant, `state_find_grant` gives false, and `state_find` asks `find_key`, so the library takes the default of the manifest.

## Functions

| Function | Result |
| --- | --- |
| `make_list` | A list value of the names, or null with no memory. |
| `state_find_grant` | Gives true and the grant when a file states it. |
