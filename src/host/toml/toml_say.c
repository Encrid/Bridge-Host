/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "say.h"
#include "toml.h"

bool toml_is_same(const value_t *a, const value_t *b)
{
    if (a->kind != b->kind)
        return (false);
    if (a->kind == MANIFEST_TEXT)
        return (!strcmp(a->text, b->text));
    if (a->kind != MANIFEST_LIST)
        return (a->flag == b->flag && a->whole == b->whole
            && a->real == b->real);
    if (a->count != b->count)
        return (false);
    for (uint32_t i = 0; i < a->count; i++)
        if (!toml_is_same(&a->items[i], &b->items[i]))
            return (false);
    return (true);
}

static void say_scalar(const value_t *v)
{
    if (v->kind == MANIFEST_TEXT)
        say_part("\"%s\"", v->text);
    if (v->kind == MANIFEST_WHOLE)
        say_part("%lld", (long long)v->whole);
    if (v->kind == MANIFEST_REAL)
        say_part("%g", v->real);
    if (v->kind == MANIFEST_FLAG)
        say_part("%s", v->flag ? "true" : "false");
}

void toml_say(const value_t *v)
{
    if (v->kind == MANIFEST_TABLE) {
        say_part("{}");
        return;
    }
    if (v->kind != MANIFEST_LIST) {
        say_scalar(v);
        return;
    }
    say_part("[");
    for (uint32_t i = 0; i < v->count; i++) {
        if (i)
            say_part(", ");
        toml_say(&v->items[i]);
    }
    say_part("]");
}
