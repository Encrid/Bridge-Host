# src/host/config/config_names.c

This file lists the distinct names stated under a key.

`manifest_add_name` of the library adds each name, with no repeat: `docs/extern/manifest/bridge_host_manifest.h.md`, *The lists of names*.
`add_names` scans one file for entries below the asked prefix and adds the next name of each.
`config_names` runs that scan over every file.

## Functions

| Function | Result |
| --- | --- |
| `config_names` | Gives the names under a key, gathered from all files. |
