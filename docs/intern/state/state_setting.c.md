# src/state/state_setting.c

This file takes the settings that a module declares.

`split_names` splits the manifest `settings` word into names.
`state_settings` refuses a setting that has no component, and takes each declared name.
`take_setting` looks a setting up in the files or the manifest, then flags a clash or an absent value.

It builds the key `config.<name>` one time, at the length of the name, in the `made_t` of the module. Thus a name has no limit of length.
It gives the key to `state_own_setting`. Its lines write the key with `config_say_key`, from the names `config` and the name.

`put_setting` resolves a path value, and makes the setting with `config_get_setting`.
The copy of the `settings` word, the settings array and each resolved path go into the `made_t` of the module.
`state_settings` frees the list of the names.

## Functions

| Function | Result |
| --- | --- |
| `state_settings` | Fills the settings array. Gives true when every setting is taken. |
