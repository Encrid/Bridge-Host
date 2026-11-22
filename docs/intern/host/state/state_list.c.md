# src/host/state/state_list.c

This file finds the grants of a module in the deployment: `read` and `write`.

`find_names` gathers a grant from the module tables, and stops at the first file that states it.
`permit_in` descends through a file and the files that it names, and tells whether one of them states the grant.
`say_names` prints a grant of the files when the host shows a module.
`make_list` makes a list value of the names, in the `made_t` of the module. The names point into the files.
When no file states a grant, `state_find_grant` gives 0, and `state_find` asks `find_key`, so the library takes the default of the manifest.

## Functions

| Function | Result |
| --- | --- |
| `make_list` | A list value of the names, or null with no memory. |
| `state_find_grant` | Gives 1 and the grant when a file states it, and 0 when no file states it. With no memory for the list, it prints `<prefix>no memory is left for its <grant>`, and gives -1. |
