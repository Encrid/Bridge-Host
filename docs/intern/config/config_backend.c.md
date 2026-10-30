# src/config/config_backend.c

This file takes the backends and their libraries into the host.

`config_backends` lists the names under `backend`, then takes each one.
At the end, it frees the list. The names stay in the configuration.

`take_backend` reads `backend.<name>.library`, the words under `content.start.<name>` and `content.arrive.<name>`, and the settings with `config_backend_settings`.
`with_suffix` appends the platform library suffix when the path lacks it, and frees the path without the suffix.
`get_arrive` maps `ask`, `load` or `leave` onto an `arrive` value, for the start rule and for the arrive rule.
`get_held` looks a held backend up by name.

## Functions

| Function | Result |
| --- | --- |
| `config_backends` | Fills the backend and held arrays. Prints `not stated: backend` when none exist. |
| `get_held` | Gives the held backend of a name, or null. |
