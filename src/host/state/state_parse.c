/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

bool state_parse([[maybe_unused]] void *ud, const char *raw, value_t *v)
{
    size_t size = strlen(raw) + sizeof("v = \n");
    char *text = calloc(size, 1);
    toml_t t = {};

    if (!text)
        return (false);
    snprintf(text, size, "v = %s\n", raw);
    if (toml_read(text, &t) && t.count == 1) {
        *v = t.entries[0].value;
        t.entries[0].value = (value_t){};
        free(text);
    } else {
        *v = (value_t){ .kind = MANIFEST_TEXT, .text = text };
        memcpy(text, raw, strlen(raw) + 1);
    }
    toml_free(&t);
    return (true);
}
