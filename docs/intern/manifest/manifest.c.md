# src/manifest/manifest.c

This file walks the words of a manifest once the section is found, and finds the default of a key.

## The words

A manifest is a run of words, and a zero byte ends each word.
`parse` walks the words in the bytes, with no copy, and demands an identity with an `abi`.
It refuses a word with no zero byte before the end of the section, since nothing would end its text.
It gives each word to `manifest_read_word`: `docs/intern/manifest/manifest_word.c.md`.
`manifest_read` first finds the section for the format, then parses it.

`get_word_end` gives the place of the zero byte that ends a word, or the end of the section if there is none.
`parse` and `manifest_get_value` both go from word to word with it.

## The defaults

`manifest_get_value` reads each word of the section, and counts the words of the key that it gets.
It points at the value of the first one. The caller refuses a key that two words state.
The value is a text that the zero byte of its word ends, in the bytes of the file.

## Functions

| Function | Result |
| --- | --- |
| `manifest_read` | Fills a `manifest_t`, or gives the reason that it refused. |
| `manifest_get_value` | Gives the count of the words of one key, and points at the value of the first. |
