# src/host_run.c

This file makes the Bridge world and runs the rounds.

## The run

`make_world` copies the `bridge_config_t` of the `deployment_t`, adds the ABI, the backends, the phases and the order, and calls `bridge_create`.
`run` loops the rounds while each of these holds:

- No call was refused, and no module asked for the end.
- The round limit holds, or `rounds` is 0.
- The run is live, or the last round had work: its look did work, or `say_record` counted a record in `said`.

`run_round` does a look, checks the named modules after the first look, runs each phase, and counts the round in `round`.
A phase that gives 2 sets `ended`, and the other phases of the round still run.
After the loop, `run` writes the line of an end that a module asked for, or else the line of the round limit.
`say_record` prints a record of the level `written` or lower, and counts a failure for a record of the level `failing` or lower.
`say_loaded` asks Bridge how many modules stay loaded, then lists them in memory of that size.

## Functions

| Function | Result |
| --- | --- |
| `run` | Runs the world to its end. Gives 0, or 1 on a failure. |
