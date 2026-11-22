/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "manifest_test.h"

static size_t get_item_end(const char *spec)
{
    size_t at = 0;
    int depth = 0;

    while (spec[at] && (depth || (spec[at] != ',' && spec[at] != ']'))) {
        depth += spec[at] == '[';
        depth -= spec[at] == ']';
        at++;
    }
    return (at);
}

static bool make_list(const char *spec, manifest_value_t *v)
{
    size_t end = 0;
    char *item = nullptr;
    bool made = true;

    *v = (manifest_value_t){ .kind = MANIFEST_LIST };
    v->items = calloc(strlen(spec) + 1, sizeof(*v->items));
    while (made && v->items && *spec && *spec != ']') {
        end = get_item_end(spec);
        item = calloc(end + 1, 1);
        if (item)
            memcpy(item, spec, end);
        made = item && test_make_value(item, &v->items[v->count]);
        free(item);
        v->count++;
        spec += end + (spec[end] == ',');
    }
    return (made && v->items);
}

static bool make_scalar(const char *spec, manifest_value_t *v)
{
    char *end = nullptr;
    char *rest = nullptr;
    int64_t whole = strtoll(spec, &end, 10);
    double real = strtod(spec, &rest);
    size_t size = strcmp(spec, "''") ? strlen(spec) + 1 : 1;

    *v = (manifest_value_t){ .kind = MANIFEST_TEXT };
    if (!strcmp(spec, "true") || !strcmp(spec, "false"))
        *v = (manifest_value_t){ .kind = MANIFEST_FLAG,
            .flag = spec[0] == 't' };
    else if (*spec && !*end)
        *v = (manifest_value_t){ .kind = MANIFEST_WHOLE, .whole = whole };
    else if (*spec && !*rest)
        *v = (manifest_value_t){ .kind = MANIFEST_REAL, .real = real };
    else
        v->text = calloc(size, 1);
    if (v->kind == MANIFEST_TEXT && v->text)
        memcpy(v->text, spec, size - 1);
    return (v->kind != MANIFEST_TEXT || v->text);
}

bool test_make_value(const char *spec, manifest_value_t *v)
{
    if (spec[0] == '[')
        return (make_list(spec + 1, v));
    if (!strcmp(spec, "{}")) {
        *v = (manifest_value_t){ .kind = MANIFEST_TABLE };
        return (true);
    }
    return (make_scalar(spec, v));
}

bool test_parse_value(void *ud, const char *text, manifest_value_t *v)
{
    const test_host_t *told = ud;

    return (!told->deaf && test_make_value(text, v));
}
