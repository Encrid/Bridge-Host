# src/host/config/config_order.c

This file takes the pairs of the 2 keys of `[order]`: `order.before` for Bridge, and `order.unload_before` for the end of a run.

`ORDER_KEYS` names the 2 keys, in the order of `enum order_kind`.
`config_order` reads the pairs of each key with `take_kind`, then asks `config_cycle` to refuse a cycle of the pairs of modules.
`take_kind` reads each pair of one key, stated in any file.

`add_order` keeps a pair once in its `pairs_t`, and uses `config_find` to catch a clash between peers.
The line of a clash names the key, then the two pairs.
`config_states_pair` finds a given pair inside the list of one key of a file.
`is_pair` tests whether two names form a pair, in either order. Thus a pair and its reverse in 2 peers clash.

## Functions

| Function | Result |
| --- | --- |
| `config_order` | Fills the 2 arrays of pairs. Gives true when every pair agrees and the pairs of modules make no cycle. |
| `config_states_pair` | Whether a file states the pair of a `pair_asked_t`, and where. |
| `take_kind` | Takes the pairs of one key. Gives true when each pair agrees. |
| `add_order` | Keeps one pair, or gives false after its line. |
| `is_pair` | Whether two names are the two names of a pair. |
