# src/host_again.c

This file runs one phase as many times as it asks, up to its cap.

`run_phase` runs a phase while Bridge gives 3 and the phase ran fewer times than its cap in `again`.
A phase that `again` does not name has the cap 1.
A run that gives 2 sets `ended`.
If the last run still gave 3, `say_again` writes the warning of the cap, or the warning of a phase that `again` does not name.

`say_again` treats each warning as a record of the level `BRIDGE_WARN`.
It writes the line if `written` allows it, and counts a failure if `failing` does.
The lines are in `readme.md`, *Messages*.

## Functions

| Function | Result |
| --- | --- |
| `run_phase` | Runs one phase, again while it asks and its cap allows. Gives the last result of Bridge. |
| `say_again` | Writes the warning of a phase that still asks again. |
