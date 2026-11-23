# src/host/host_go.c

This file runs or shows the configuration of a path.

`host_go` enters the folder of the main file with `config_go_beside`, and `take_config` reads, checks and takes the keys.
Before the backends, `take_config` keeps `<name>.d/copies` of `state_path` in `copies` of `host_t`, for the setting of the backend `native`.
A run then takes the lock and calls `run`; a show calls `show_config` instead.
After `run`, `host_go` ends the lock with `os_unlock`: `docs/intern/host/host_lock.c.md`.
`config_go_beside` writes into the path, so each call needs its own copy of it.
`main` calls it 1 time, and the test program `host-sweep` 1 time in each turn of its sweep: `tests/cases/host/sweep.toml`.

`host-sweep` refuses `realloc` in turn, or the call that its 2nd argument names, such as `malloc`.

## Functions

| Function | Result |
| --- | --- |
| `host_go` | Runs or shows a configuration. Gives the status of the run, or of the show. |
