# src/manifest/manifest_words.c

This file tells the host the words that only the manifest states: `config`, `settings`, `pinned` and `reloadable`.

`manifest_own_words` tells `MANIFEST_TOLD_NO_WORD` for `config`, `pinned` or `reloadable` when the manifest does not state it.
Then `tell_words` tells the value of each of the 4 words, so the host can show them.

## Functions

| Function | Result |
| --- | --- |
| `tell_missing` | Tells a word that the manifest does not state. |
| `tell_words` | Tells the value of each word. |
| `manifest_own_words` | Gives true when the manifest states `config`, `pinned` and `reloadable`. |
