# src/config/config_checks.c

This file reads and checks the `[checks]` table, which holds the diagnostic switches of a world.

`take_checks` copies the `checks.profile` and `checks.validate` flags into the host, with `take_check` for each flag.
`take_check` leaves a flag off and writes no line when no file states it.
If two files state different values, `take_check` writes the line of `config_say_found` and gives false.
`config_check_checks` allows `profile` and `validate`, each a flag at depth two, and refuses a key or a depth that is not that.
The local file may not carry a `checks` key: a world fixes its checks when it is made.

## Functions

| Function | Result |
| --- | --- |
| `take_check` | Reads one switch into the host. Gives false if two files state different values. |
| `take_checks` | Reads the profile and validate switches into the host. Gives false if one of them is refused. |
| `config_check_checks` | Checks a key of the checks table, or says why not. |
