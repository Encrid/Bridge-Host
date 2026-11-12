# src/host/config/config_unloads.c

This file checks the pairs of modules of `order.unload_before`: `docs/extern/host/readme.md`, *The world*.

## The check of a value

`config_check_unloads` checks the value of the key in one file:

1. `CHECK_MODULE_PAIRS` checks that each item is a pair of names.
2. Each name has a `:` and no `@`. Thus it is `<namespace>:<name>`, with no version.
3. No pair names one module twice.

## The cycles

`config_cycle` checks the pairs of all the files, after `config_order` took them:

1. Each pair is kept at the start.
2. `drop_once` drops each kept pair that `is_end` finds at an end. At an end, no kept pair goes into its first module, or none leaves its second module.
3. The drops go on until a pass drops nothing. Each pair that stays is on a cycle, or between two cycles.
4. If a pair stays, `say_cycle` writes the refusal of the first one, at the place that states it.

A pair of a cycle can name modules that are not loaded. The host refuses it, because no order of the unloads fits it.

## Functions

| Function | Result |
| --- | --- |
| `config_check_unloads` | Why the value of the key is refused, or none. |
| `config_cycle` | True when the pairs make no cycle. Else false, after the refusal. |
| `drop_once` | The count of the pairs that one pass drops. |
| `is_end` | Whether a kept pair is at an end of the kept pairs. |
| `say_cycle` | Writes `refused: <file>:<line>: order.unload_before: <a> and <b> make a cycle`. |
