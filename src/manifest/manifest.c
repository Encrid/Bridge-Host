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

static size_t get_word_end(const char *text, size_t at, size_t size)
{
    return (at + strnlen(text + at, size - at));
}

static const char *read_word(manifest_t *m, const char *word, bool first)
{
    static char why[160];
    const char *value = strchr(word, '=');
    size_t len = strcspn(word, "=");

    if (first && (!strchr(word, ':') || !strchr(word, '@') || value))
        return ("its manifest does not begin with who it is");
    if (first && len > IDENTITY_MOST)
        return ("its identity has more than 127 bytes");
    if (first) {
        m->identity = word;
        return (nullptr);
    }
    if (value && manifest_take(m, word, len, value + 1) >= 0)
        return (nullptr);
    if (len > WORD_SHOWN_MOST)
        len = WORD_SHOWN_MOST;
    snprintf(why, sizeof(why), "states a %.*s that cannot be", (int)len, word);
    return (why);
}

static const char *parse(const char *text, size_t size, manifest_t *m)
{
    const char *why = nullptr;
    size_t at = 0;
    size_t end = 0;

    while (!why && at < size && text[at]) {
        end = get_word_end(text, at, size);
        if (end == size)
            return ("its manifest has a word with no zero byte at its end");
        why = read_word(m, text + at, !at);
        at = end + 1;
    }
    if (why)
        return (why);
    if (!m->identity[0])
        return ("its manifest is empty");
    return (m->abi ? nullptr : "states no abi");
}

const char *manifest_read(const unsigned char *bytes, size_t len,
    manifest_t *m)
{
    size_t at = 0;
    size_t size = 0;

    *m = (manifest_t){ .identity = "", .config = "", .settings = "" };
    m->backend = manifest_find(bytes, len, &at, &size);
    if (!m->backend)
        return ("neither a wasm module nor a library");
    if (!size)
        return ("no manifest");
    return (parse((const char *)bytes + at, size, m));
}

const char *manifest_get_value(const unsigned char *bytes, size_t len,
    const char *key, size_t *size)
{
    size_t name = strlen(key);
    size_t at = 0;
    size_t left = 0;
    const char *text = nullptr;
    size_t end = 0;

    if (!manifest_find(bytes, len, &at, &left))
        return (nullptr);
    text = (const char *)bytes + at;
    for (size_t word = 0; word < left && text[word]; word = end + 1) {
        end = get_word_end(text, word, left);
        if (end - word > name && text[word + name] == '='
            && !memcmp(text + word, key, name)) {
            *size = end - word - name - 1;
            return (text + word + name + 1);
        }
    }
    return (nullptr);
}
