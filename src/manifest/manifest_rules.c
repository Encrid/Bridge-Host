/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "manifest_inside.h"

static const manifest_rule_t RULES[] = {
    [MANIFEST_CHECK_FLAG] = { MANIFEST_ALLOW_FLAG, 0, 0, "",
        "true or false" },
    [MANIFEST_CHECK_COUNT] = { MANIFEST_ALLOW_WHOLE, 1, UINT32_MAX, "",
        "a whole number, from 1" },
    [MANIFEST_CHECK_LIMIT] = { MANIFEST_ALLOW_WHOLE | MANIFEST_ALLOW_WORD, 1,
        INT64_MAX, "none ", "a whole number, from 1, or \"none\"" },
    [MANIFEST_CHECK_LEVEL] = { MANIFEST_ALLOW_WHOLE, 1, 7, "",
        "a level, from 1 to 7" },
    [MANIFEST_CHECK_NAMES] = { MANIFEST_ALLOW_NAMES, 0, 0, "",
        "a list of names" },
    [MANIFEST_CHECK_NAMES_NONE] = { MANIFEST_ALLOW_NAMES
        | MANIFEST_ALLOW_WORD, 0, 0, "none ",
        "a list of names, or \"none\"" },
    [MANIFEST_CHECK_ERROR] = { MANIFEST_ALLOW_WORD, 0, 0, "fail skip ",
        "\"fail\" or \"skip\"" },
    [MANIFEST_CHECK_TEXT] = { MANIFEST_ALLOW_TEXT, 0, 0, "", "text" },
    [MANIFEST_CHECK_SETTING] = { MANIFEST_ALLOW_FLAG | MANIFEST_ALLOW_WHOLE
        | MANIFEST_ALLOW_REAL | MANIFEST_ALLOW_TEXT | MANIFEST_ALLOW_EMPTY,
        INT64_MIN, INT64_MAX, "",
        "text, a number, true or false, or { path = \"...\" }" },
    [MANIFEST_CHECK_AUTHOR] = { 0, 0, 0, "",
        "only a module's author states it, in its file" }
};

const manifest_rule_t *manifest_get_rule(enum manifest_check check)
{
    return (&RULES[check]);
}
