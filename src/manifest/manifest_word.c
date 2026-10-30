/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "manifest_inside.h"

constexpr size_t IDENTITY_MOST = 127;
constexpr size_t WORD_SHOWN_MOST = 60;

static const char *read_identity(manifest_t *m, const char *word)
{
    if (!strchr(word, ':') || !strchr(word, '@') || strchr(word, '='))
        return ("its manifest does not begin with who it is");
    if (strlen(word) > IDENTITY_MOST)
        return ("its identity has more than 127 bytes");
    m->identity = word;
    return (nullptr);
}

const char *manifest_read_word(manifest_t *m, const char *word, bool first)
{
    static char why[160];
    const char *value = strchr(word, '=');
    size_t len = strcspn(word, "=");
    int taken = -1;

    if (first)
        return (read_identity(m, word));
    if (value)
        taken = manifest_take(m, word, len, value + 1);
    if (taken >= 0)
        return (nullptr);
    if (taken == -2)
        snprintf(why, sizeof(why), "states %.*s twice", (int)len, word);
    else
        snprintf(why, sizeof(why), "states a %.*s that cannot be",
            (int)(len < WORD_SHOWN_MOST ? len : WORD_SHOWN_MOST), word);
    return (why);
}
