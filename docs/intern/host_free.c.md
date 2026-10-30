# src/host_free.c

This file frees the memory of the host.

`host_free` frees the library path and the settings of each backend, each content path and each ignored path.
It frees the path and the `handed_t` of each file, the array of the slots, and the list of the found files.
Then it frees the arrays of the host, and the configuration.
The phases and the order point into the configuration, so `host_free` frees only their arrays.
`handed_free` frees the bytes of a `handed_t`, and the `handed_t`.

## Functions

| Function | Result |
| --- | --- |
| `host_free` | Frees what the host allocated. |
| `handed_free` | Frees a `handed_t` and its bytes. It takes null. |
