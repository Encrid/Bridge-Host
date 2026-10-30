/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool is_yes(void)
{
    static const char YES[] = "yes";
    size_t at = 0;
    bool same = true;

    for (int c = getchar(); c != EOF && c != '\n'; c = getchar()) {
        same = same && at < sizeof(YES) - 1 && c == YES[at];
        at++;
    }
    return (same && at == sizeof(YES) - 1);
}

static bool is_trusted(const file_t *f, const char *backend)
{
    say("? trust %s, %s, %s\n", f->of->who.identity, backend, f->path);
    return (is_yes());
}

bool may_arrive(const file_t *f, const held_t *held, enum arrive rule)
{
    bool may = rule == ARRIVE_LOAD;

    if (rule == ARRIVE_ASK)
        may = is_trusted(f, held->name);
    if (!may)
        say("not loaded: %s: %s, %s, not trusted\n", f->path,
            f->of->who.identity, held->name);
    return (may);
}
