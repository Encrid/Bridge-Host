# src/manifest/manifest_rules.c

This file holds the rule of each check of `manifest_check`: `docs/extern/manifest/bridge_host_manifest.h.md`, *The values*.
The table `RULES` takes the check as its index. `MANIFEST_CHECK_AUTHOR` allows no value, so each value gives its reason.

The top of a level is `BRIDGE_LEVEL_MOST` of `bridge.h`, so the rule follows Bridge. Its reason names the number 7.
A `static_assert` checks that `BRIDGE_LEVEL_MOST` is 7, so a change of Bridge stops the build until the reason follows it.

## Functions

| Function | Result |
| --- | --- |
| `manifest_get_rule` | The rule of a check. |
