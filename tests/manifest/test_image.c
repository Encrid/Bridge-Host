/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifest_test.h"

constexpr size_t WASM_HEAD = 11;
constexpr size_t WASM_NAME = 16;

static unsigned char *read_file(const char *path, size_t *len)
{
    FILE *file = fopen(path, "rb");
    unsigned char *bytes = nullptr;
    long size = 0;

    if (!file)
        return (nullptr);
    if (!fseek(file, 0, SEEK_END))
        size = ftell(file);
    rewind(file);
    if (size > 0)
        bytes = malloc((size_t)size);
    if (bytes)
        *len = fread(bytes, 1, (size_t)size, file);
    fclose(file);
    return (bytes);
}

static size_t put_word(unsigned char *into, const char *word)
{
    size_t len = strlen(word);
    bool cut = len && word[len - 1] == '^';

    len -= cut;
    for (size_t i = 0; i < len; i++)
        into[i] = word[i] == '~' ? ' ' : (unsigned char)word[i];
    return (len + !cut);
}

static unsigned char *make_wasm(char **words, int count, size_t *len)
{
    static const unsigned char HEAD[] = { 0, 'a', 's', 'm', 1, 0, 0, 0, 0 };
    static const char NAME[] = "\x0f" "bridge:manifest";
    size_t size = WASM_HEAD + WASM_NAME;
    unsigned char *bytes = nullptr;

    for (int i = 0; i < count; i++)
        size += strlen(words[i]) + 1;
    bytes = calloc(size, 1);
    if (!bytes)
        return (nullptr);
    memcpy(bytes, HEAD, sizeof(HEAD));
    memcpy(bytes + WASM_HEAD, NAME, WASM_NAME);
    *len = WASM_HEAD + WASM_NAME;
    for (int i = 0; i < count; i++)
        *len += put_word(bytes + *len, words[i]);
    bytes[9] = (unsigned char)(((*len - WASM_HEAD) & 0x7f) | 0x80);
    bytes[10] = (unsigned char)((*len - WASM_HEAD) >> 7);
    return (bytes);
}

void test_load(test_module_t *t, char **words, int count)
{
    if (count && words[0][0] == '#')
        t->bytes = test_make_image(words, count, &t->len);
    else if (count && words[0][0] == '@')
        t->bytes = read_file(words[0] + 1, &t->len);
    else
        t->bytes = make_wasm(words, count, &t->len);
    t->m.bytes = t->bytes;
    t->m.len = t->len;
}
