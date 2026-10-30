# src/main.c

This file is the entry point: it reads the arguments and runs or shows a configuration.

`main` weighs the arguments.
One argument runs a configuration; `--show-config` with a path shows one.
`say_usage` prints the two usage lines and returns 2.

## The signals

Before it reads a file, `main` gives the line `STOP` to `os_signal_catch` of Bridge-OS.
At the first signal, the layer writes that line at once, and sets a flag. A second signal ends the process with the status 1.
`run` reads the flag between two rounds: `docs/intern/host_run.c.md`.
If the host cannot catch the signals, `main` writes `cannot catch the signals`, and gives 1.

## The work

`go` enters the folder of the main file with `config_go_beside`, and `take_config` reads, checks and takes the keys.
A run then takes the lock and calls `run`; a show calls `show_config` instead.
At the end, `main` calls `host_free`, and gives the result of the run or of the show.

## Functions

| Function | Result |
| --- | --- |
| `main` | Runs or shows a configuration. Gives 0, 1 or 2. |
