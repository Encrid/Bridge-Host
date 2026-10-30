/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "say.h"

static bool is_bare(const char *name)
{
    size_t len = strspn(name, "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-");

    return (len && !name[len]);
}

const char *config_say_key(const char *const *names, uint32_t depth,
    char *out, size_t size)
{
    size_t used = 0;
    const char *form = nullptr;

    out[0] = 0;
    for (uint32_t i = 0; i < depth && used < size; i++) {
        form = is_bare(names[i]) ? "%s%s" : "%s\"%s\"";
        used += (size_t)snprintf(out + used, size - used, form,
            i ? "." : "", names[i]);
    }
    return (out);
}

bool config_say_found(const config_t *c, const char *key, const found_t *f,
    const char *prefix)
{
    if (!f->at.value && !f->other.value)
        say("%snot stated: %s\n", prefix, key);
    if (!f->other.value)
        return (f->at.value != nullptr);
    say_part("%srefused: %s: ", prefix, key);
    toml_say(f->at.value);
    say_part(" in %s:%u, ", c->files[f->at.file].path, f->at.line);
    toml_say(f->other.value);
    say(" in %s:%u\n", c->files[f->other.file].path, f->other.line);
    return (false);
}

void config_say_where(const config_t *c, const said_t *said)
{
    if (said->file == UINT32_MAX)
        say_part("  its file");
    else
        say_part("  %s:%u", c->files[said->file].path, said->line);
}

void config_say_over(const config_t *c, states_fn *states,
    const void *asked, const said_t *at)
{
    said_t other = {};

    for (uint32_t i = 0; i < c->count; i++)
        if (i != at->file && states(&c->files[i], asked, &other))
            say_part(", over %s:%u", c->files[i].path, other.line);
}
