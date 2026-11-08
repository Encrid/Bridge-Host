# src/host/state/state_undeclared.c

This file gives the function `undeclared` of the host to the library: the settings that a file states and that the module does not declare.

`has_declared` tests whether a name sits in the declared list.
`say_undeclared_in` prints `it takes no config.<name>, stated in <file>:<line>` for each stray setting.
`state_undeclared` runs that scan over every file, with the names that the library gives.

## Functions

| Function | Result |
| --- | --- |
| `state_undeclared` | Prints each undeclared setting and gives the count. |
