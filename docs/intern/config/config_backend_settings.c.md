# src/config/config_backend_settings.c

This file takes the settings of one backend into the host.

`config_backend_settings` lists the names under `backend.<name>`, and takes each one but `library`.
`take_setting` finds the value of a setting over the files, and makes it a `bridge_setting_t` with `config_get_setting`.
The host checks only the form of a setting. A setting that is missing, or that is not known, is refused by the backend.
The array of the settings goes into the `held_t` and into the `bridge_backend_t` of the backend, and `host_free` frees it.
The key and a text value point into the configuration.

## Functions

| Function | Result |
| --- | --- |
| `config_backend_settings` | Fills the settings of a backend. Gives true when each setting is taken. |
