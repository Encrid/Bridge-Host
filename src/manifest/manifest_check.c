/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "manifest_inside.h"

static bool is_word(const char *text, const char *words)
{
    size_t len = strlen(text);
    const char *at = len ? strstr(words, text) : nullptr;

    return (at && (at == words || at[-1] == ' ') && at[len] == ' ');
}

static bool is_names(const manifest_value_t *v, uint32_t count)
{
    bool names = v->kind == MANIFEST_LIST && (!count || v->count == count);

    for (uint32_t i = 0; names && i < v->count; i++)
        names = v->items[i].kind == MANIFEST_TEXT && v->items[i].text[0];
    return (names);
}

static bool is_pairs(const manifest_value_t *v)
{
    bool pairs = v->kind == MANIFEST_LIST && v->count;

    for (uint32_t i = 0; pairs && i < v->count; i++)
        pairs = is_names(&v->items[i], 2);
    return (pairs);
}

static bool is_allowed(const manifest_value_t *v, const manifest_rule_t *rule)
{
    switch (v->kind) {
        case MANIFEST_FLAG:
            return (rule->allow & MANIFEST_ALLOW_FLAG);
        case MANIFEST_WHOLE:
            return ((rule->allow & MANIFEST_ALLOW_WHOLE)
                && v->whole >= rule->least && v->whole <= rule->most);
        case MANIFEST_REAL:
            return (rule->allow & MANIFEST_ALLOW_REAL);
        case MANIFEST_TEXT:
            return (((rule->allow & MANIFEST_ALLOW_WORD) && is_word(v->text,
                rule->words)) || ((rule->allow & MANIFEST_ALLOW_TEXT)
                && (v->text[0] || (rule->allow & MANIFEST_ALLOW_EMPTY))));
        default:
            return (((rule->allow & MANIFEST_ALLOW_NAMES) && is_names(v, 0))
                || ((rule->allow & MANIFEST_ALLOW_PAIRS) && is_pairs(v)));
    }
}

const char *manifest_check_value(const manifest_value_t *v,
    const manifest_rule_t *rule)
{
    return (is_allowed(v, rule) ? nullptr : rule->why);
}
