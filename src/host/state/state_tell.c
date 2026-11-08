/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void say_joined(const char *prefix, const manifest_told_t *told)
{
    say_part("%sits file states ", prefix);
    for (uint32_t i = 0; i < told->depth; i++)
        say_part("%s%s", i ? "." : "", told->names[i]);
    if (told->why)
        say(": %s\n", told->why);
    else
        say(" twice\n");
}

static void say_value(const stating_t *s, const manifest_told_t *told)
{
    const keyed_t key = { told->names, told->depth };
    const found_t f = { .at = { told->value, UINT32_MAX, 0 } };
    const shown_t how = { "  ", nullptr, nullptr };

    if (s->show)
        config_say_value(&s->h->config, &key, &f, &how);
}

static void say_not_stated(const char *prefix, const manifest_told_t *told)
{
    say_part("%snot stated: ", prefix);
    config_say_key(told->names, told->depth);
    say("\n");
}

void state_tell(void *ud, const manifest_told_t *told)
{
    const stating_t *s = ud;

    switch (told->kind) {
        case MANIFEST_TOLD_VALUE:
            say_value(s, told);
            break;
        case MANIFEST_TOLD_NOT_STATED:
            say_not_stated(s->prefix, told);
            break;
        case MANIFEST_TOLD_TWICE:
        case MANIFEST_TOLD_WRONG:
            say_joined(s->prefix, told);
            break;
        case MANIFEST_TOLD_NO_WORD:
            say("%sits file states no %s\n", s->prefix, told->names[0]);
            break;
        default:
            say("%sit names no component, and settings\n", s->prefix);
    }
}
