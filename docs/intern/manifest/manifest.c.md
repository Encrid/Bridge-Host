# src/manifest/manifest.c

This file reads the words of a manifest once the section is found.

## The words

A manifest is a run of words, and a zero byte ends each word.
The first word is the identity. It carries a `:` and an `@` and no `=`, and has at most 127 bytes, the limit of Bridge.
`read_word` checks the `:`, the `@` and the `=` of the first word before its length.
`parse` walks the words in the bytes, with no copy, and demands an identity with an `abi`.
It refuses a word with no zero byte before the end of the section, since nothing would end its text.

`get_word_end` gives the place of the zero byte that ends a word, or the end of the section if there is none.
`parse` and `manifest_get_value` both go from word to word with it.

`read_word` points `identity` at the first word.
It splits a later word at its first `=`, and gives the two parts to `manifest_take`.
The reason of a refused word shows the part before its first `=`, and at most 60 bytes of that part.
`manifest_read` first finds the section for the format, then parses it.
`readme.md`, *Messages*, quotes each reason of a refusal.

## Functions

| Function | Result |
| --- | --- |
| `manifest_read` | Fills a `manifest_t`, or gives the reason that it refused. |
| `manifest_get_value` | Locates one key among the words and points at its raw value. |
