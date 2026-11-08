# src/host/state/state_tell.c

This file gives the function `tell` of the host to the library: the lines of a value of the manifest and of each fault.

| `kind` | Line |
| --- | --- |
| `MANIFEST_TOLD_VALUE` | When the host shows a module: `  <key> = <value>  its file`. |
| `MANIFEST_TOLD_NOT_STATED` | `not stated: <key>`, after the prefix. |
| `MANIFEST_TOLD_TWICE` | `its file states <key> twice`, after the prefix. |
| `MANIFEST_TOLD_WRONG` | `its file states <key>: <reason>`, after the prefix. |
| `MANIFEST_TOLD_NO_WORD` | `its file states no <word>`, after the prefix. |
| `MANIFEST_TOLD_NO_COMPONENT` | `it names no component, and settings`, after the prefix. |

The prefix is the prefix of the `stating_t`. `docs/extern/host/readme.md`, *Messages*, quotes each line.

## Functions

| Function | Result |
| --- | --- |
| `say_joined` | Writes the line of a default that the manifest states twice or in a wrong form. |
| `say_value` | Writes a value of the manifest when the host shows a module. |
| `say_not_stated` | Writes the line of a key that nothing states. |
| `state_tell` | Writes the line of what the library tells. |
