# src/manifest/manifest_word.c

This file reads one word of a manifest.

## The identity

The first word is the identity. It carries a `:` and an `@` and no `=`, and has at most 127 bytes, the limit of Bridge.
`read_identity` checks the `:`, the `@` and the `=` of the first word before its length.
Then it points `identity` at the word.

## A later word

`manifest_read_word` splits a later word at its first `=`, and gives the two parts to `manifest_take`.
A word with no `=` is refused, and so is a word whose value cannot be.
The reason of such a word shows the part before its first `=`, and at most 60 bytes of that part.
A word whose key `manifest_take` took before gives the reason `states <key> twice`.
`readme.md`, *Messages*, quotes each reason of a refusal.

## Functions

| Function | Result |
| --- | --- |
| `manifest_read_word` | Gives null for a word that is taken or unknown, else the reason that it refused. |
