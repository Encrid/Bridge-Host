/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "config.h"

static const manifest_rule_t RULES[] = {
    [CHECK_ARRIVE - CHECK_ARRIVE] = { MANIFEST_ALLOW_WORD, 0, 0,
        "ask load leave ", "\"ask\", \"load\" or \"leave\"" },
    [CHECK_ROUNDS - CHECK_ARRIVE] = { MANIFEST_ALLOW_WHOLE, 0, UINT32_MAX, "",
        "a whole number, from 0 to 4294967295" },
    [CHECK_PAIRS - CHECK_ARRIVE] = { MANIFEST_ALLOW_PAIRS, 0, 0, "",
        "a list of pairs of systems" },
    [CHECK_SIZE - CHECK_ARRIVE] = { MANIFEST_ALLOW_WHOLE, 1, INT64_MAX, "",
        "a whole number, from 1" },
    [CHECK_VALUE - CHECK_ARRIVE] = { MANIFEST_ALLOW_FLAG
        | MANIFEST_ALLOW_WHOLE | MANIFEST_ALLOW_REAL | MANIFEST_ALLOW_TEXT
        | MANIFEST_ALLOW_EMPTY, INT64_MIN, INT64_MAX, "",
        "text, a number, true or false" },
    [CHECK_RECORD - CHECK_ARRIVE] = { MANIFEST_ALLOW_WHOLE, 0, 7, "",
        "a level, from 0 to 7" }
};

const char *config_check_value(const value_t *v, enum check check)
{
    if (check < CHECK_ARRIVE)
        return (manifest_check_value(v,
            manifest_get_rule((enum manifest_check)check)));
    return (manifest_check_value(v, &RULES[check - CHECK_ARRIVE]));
}
