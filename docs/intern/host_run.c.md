# src/host_run.c

This file makes the Bridge world and runs the rounds.

## The run

`make_world` copies the `bridge_config_t` of the `deployment_t`, adds the ABI, the backends, the phases and the order, and calls `bridge_create`.
`run` loops the rounds while each of these holds:

- No call was refused, no module asked for the end, and `os_signal_is_caught` tells of no signal.
- The round limit holds, or `rounds` is 0.
- The run is live, or the last round had work: its look did work, or `say_record` counted a record in `said`.

`run_round` does a look, checks the named modules after the first look, runs each phase, and counts the round in `round`.
A phase that gives 2 sets `ended`, and the other phases of the round still run.
After the loop, `say_ended` writes the line of an end that a module asked for, of a signal, or of the round limit.
A signal stops the run only between two rounds, so the round that runs finishes.
`say_record` prints a record of the level `written` or lower, and counts a failure for a record of the level `failing` or lower.
After the line of the end, `run` calls `say_end` of `src/host_end.c`, then `bridge_destroy`.

## Functions

| Function | Result |
| --- | --- |
| `run` | Runs the world to its end. Gives 0, or 1 on a failure. |
