# src/manifest/manifest_check.c

This file checks a value against a rule, by the bits of `allow` of the rule.

- `is_word` finds the text as a whole word of `words`, each word followed by a space.
- `is_names` tells whether each item of a list is a name. With a count, it also checks the count.
- `is_pairs` tells whether a list, not empty, holds lists of 2 names.
- `is_allowed` picks the test by the kind of the value.

## Functions

| Function | Result |
| --- | --- |
| `is_word` | Whether a text is one of the words. |
| `is_names` | Whether a value is a list of names. |
| `is_pairs` | Whether a value is a list of pairs of names. |
| `is_allowed` | Whether a rule allows a value. |
| `manifest_check_value` | No text, or `why` of the rule. |
