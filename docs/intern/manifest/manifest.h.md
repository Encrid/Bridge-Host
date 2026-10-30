# src/manifest/manifest.h

This header defines `manifest_t` and declares the two readers of a manifest.
A `manifest_t` holds the identity of a module, its backend, and the author keys `config`, `settings`, `abi`, `pinned` and `reloadable`.
The mask `taken` holds a mark of `enum manifest_word` for each author word that the manifest stated.

## The words in the bytes

`identity`, `config` and `settings` point into the bytes of the file, at the text of their words.
The zero byte that ends each word ends each of them too.
A word that the manifest does not state gives an empty text, not a null pointer.
The bytes outlive the manifest, since one `handed_t` holds both, and `handed_free` frees both.
Thus the manifest copies no word, and the identity is the one word with a limit of length: `docs/intern/manifest/manifest.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `manifest_read` | Fills a `manifest_t` from the bytes of a file. Gives a reason on refusal, else null. |
| `manifest_get_value` | Counts the words of one key in the manifest, and gives the value of the first of them. |
