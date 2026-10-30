/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "manifest_inside.h"

static size_t get_word_end(const char *text, size_t at, size_t size)
{
    return (at + strnlen(text + at, size - at));
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
        why = manifest_read_word(m, text + at, !at);
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

uint32_t manifest_get_value(const unsigned char *bytes, size_t len,
    const char *key, const char **value)
{
    size_t name = strlen(key);
    size_t at = 0;
    size_t left = 0;
    const char *text = nullptr;
    size_t end = 0;
    uint32_t count = 0;

    if (!manifest_find(bytes, len, &at, &left))
        return (0);
    text = (const char *)bytes + at;
    for (size_t word = 0; word < left && text[word]; word = end + 1) {
        end = get_word_end(text, word, left);
        if (end - word <= name || text[word + name] != '='
            || memcmp(text + word, key, name))
            continue;
        if (!count)
            *value = text + word + name + 1;
        count++;
    }
    return (count);
}
