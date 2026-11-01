# src/host_slot.c

This file finds the slot of a path, and keeps a path in its slot.

`find_file` gives the slot that holds the path, else the first free slot, else null.
`keep_path` copies the path into a free slot, and keeps the path of a slot that holds one.
A slot with a path counts as taken, also when the host could not read its file.
`drop_gone` unloads the module of each file that the last read of its path did not see.
Then the slot gives back its path and its `handed_t`, so it is free again.

## Functions

| Function | Result |
| --- | --- |
| `find_file` | Gives the slot of a path, or a free slot, or null. |
| `keep_path` | Gives true when the slot holds the path. |
| `drop_gone` | Frees the slots whose files are gone, and unloads their modules. |
