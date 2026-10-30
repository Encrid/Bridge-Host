# src/host_meet.c

This file meets a file: it reads the manifest and either records a new module or replaces a loaded one.

## The meeting

`find_file` gives the slot of a path, or a free slot.
`meet` reads the manifest; a loaded file goes to `replace`, and a new file keeps its path and bytes.
`meet` frees the `handed_t` of a slot before it puts the new one there.
A bad manifest prints `not a module` or `not loaded`.

## The replacement

`replace` gives an `enum replaced` for each changed file of a loaded module, in these steps:

1. `keep_old` keeps the old module when the new manifest is refused, or when the old manifest does not state `reloadable=true`.
2. `reload_found` refuses a module whose ABI differs.
3. It takes `hot_reload` of the new module through `state_find_key`, from the files or from the manifest.
4. If `hot_reload` is `false`, the old module stays, and no failure counts. The host asks Bridge for nothing.
5. Else it states the new module, and calls `bridge_reload`. Then it frees the memory of the statement.

Each line of the statement starts with `not replaced: <file>: <identity>: `, which `state_keep_prefix` makes.

| `enum replaced` | The host |
| --- | --- |
| `REPLACED` | Writes `replaced:`, frees the old `handed_t`, and keeps the new one. |
| `REPLACE_REFUSED` | Writes `was refused`, counts a failure, and frees the new `handed_t`. |
| `REPLACE_NOT_HOT` | Writes `its hot_reload is false`, and frees the new `handed_t`. |
| `REPLACE_KEPT` | Frees the new `handed_t`. `keep_old` wrote the reason. |

## Functions

| Function | Result |
| --- | --- |
| `find_file` | Gives the slot of a path, or a free slot, or null. |
| `meet` | Records a new module and gives true, or replaces a loaded one and gives false. |
