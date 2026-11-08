# src/host/state/state_setting_find.c

This file matches a setting statement of the deployment.

`state_is_setting` tests whether an entry is `module.<identity>.config.<name>` or its `path`.
`state_states_setting` finds such an entry in one file.
`state_is_path` tells whether the found value came as the `path` form.

## Functions

| Function | Result |
| --- | --- |
| `state_is_setting` | Gives true when an entry is a setting of the module. |
| `state_states_setting` | Gives true when a file states the setting. |
| `state_is_path` | Gives true when the value is the `path` form. |
