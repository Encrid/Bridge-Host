# src/host_arrive.c

This file asks whether an arriving module may load.

`may_arrive` loads at once when the arrive rule is `load`, and asks when the rule is `ask`.
`is_trusted` prints `? trust <identity>, <backend>, <file>`, and `is_yes` reads the answer.
A refusal prints `not loaded: <file>: <identity>, <backend>, not trusted`.

## The answer

The answer is one whole line: each character up to the line feed, or up to the end of the input.
`is_yes` reads the line one character at a time, and keeps no copy of it. Thus a line has no limit of length.
The module may load only if the line is `yes`, with nothing before it and nothing after it.
The next question reads the next line, and never the rest of a long answer.

## Functions

| Function | Result |
| --- | --- |
| `may_arrive` | Gives true when the module may load under its arrive rule. |
