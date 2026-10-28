# src/state/state_setting.c

This file takes the settings that a module declares.

`split_names` splits the manifest `settings` word into names.
`state_settings` refuses a setting that has no component, and takes each declared name.
`take_setting` looks a setting up in the files or the manifest, then flags a clash or an absent value.
It builds the key `config.<name>` one time, at the length of the name, in the `made_t` of the module. Thus a name has no limit of length.
It writes the key in its lines, and gives it to `state_own_setting`.

`put_setting` writes the setting as a string, an integer, a float, or a boolean, and resolves a path value.
The copy of the `settings` word, the settings array and each resolved path go into the `made_t` of the module.
`state_settings` frees the list of the names.

## Functions

| Function | Result |
| --- | --- |
| `state_settings` | Fills the settings array. Gives true when every setting is taken. |
