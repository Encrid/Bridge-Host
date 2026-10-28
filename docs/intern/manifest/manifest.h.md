# src/manifest/manifest.h

This header defines `manifest_t` and declares the two readers of a manifest.
A `manifest_t` holds the identity of a module, its backend, and the author keys `config`, `settings`, `abi`, `pinned` and `reloadable`.
The flags `has_pinned` and `has_reloadable` record whether the manifest stated each one.

## The words in the bytes

`identity`, `config` and `settings` point into the bytes of the file, at the text of their words.
The zero byte that ends each word ends each of them too.
The bytes outlive the manifest, since one `handed_t` holds both, and `handed_free` frees both.
Thus the manifest copies no word, and no word has a limit of length but the identity: `docs/intern/manifest/manifest.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `manifest_read` | Fills a `manifest_t` from the bytes of a file. Gives a reason on refusal, else null. |
| `manifest_get_value` | Points at the raw value of one key in the manifest, and reports its size. |
