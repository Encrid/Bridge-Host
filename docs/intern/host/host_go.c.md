# src/host/host_go.c

This file runs or shows the configuration of a path.

`host_go` enters the folder of the main file with `config_go_beside`, and `take_config` reads, checks and takes the keys.
A run then takes the lock and calls `run`; a show calls `show_config` instead.
After `run`, `host_go` ends the lock with `os_unlock`: `docs/intern/host/host_lock.c.md`.
`config_go_beside` writes into the path, so each call needs its own copy of it.
`main` calls it 1 time, and the test program `host-sweep` 1 time in each turn of its sweep: `tests/cases/host/sweep.toml`.

## Functions

| Function | Result |
| --- | --- |
| `host_go` | Runs or shows a configuration. Gives the status of the run, or of the show. |
