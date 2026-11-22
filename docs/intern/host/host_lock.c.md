# src/host/host_lock.c

This file takes the run lock of a configuration.

`take_lock` makes the `<name>.d` folder, then locks `<name>.d/lock` through `os_lock`.
It prints `refused: <name>.d is no folder` when the folder cannot be made.
It prints `refused: <name> is being run` only when another process holds the lock: `os_lock` of mosl gives 1 for a held lock only.
It prints `cannot lock '<path>'` when the open fails, or when the system refuses the lock for another reason, for which `os_lock` gives -1.

`take_lock` frees the path when it returns, and gives the handle of the lock in `*lock`.
`go` of `src/host/main.c` keeps the handle while the host runs, and gives it to `os_unlock` before the end of the host.
`os_unlock` ends the lock at once, also on Windows, where the end of a lock after the end of the process can take some time.

## Functions

| Function | Result |
| --- | --- |
| `take_lock` | Takes the run lock. Gives true when the host holds it, with its handle in `*lock`. Else `*lock` is null. |
