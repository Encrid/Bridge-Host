/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "manifest_test.h"

static void put_text(unsigned char *bytes, size_t size, size_t at,
    const char *text)
{
    for (size_t i = 0; text[i] && at + i < size; i++)
        bytes[at + i] = text[i] == '~' ? ' ' : (unsigned char)text[i];
}

static void put_patch(unsigned char *bytes, size_t size, const char *word)
{
    char *rest = nullptr;
    size_t at = strtoul(word, &rest, 16);
    unsigned int byte = 0;

    if (*rest == '=') {
        put_text(bytes, size, at, rest + 1);
        return;
    }
    rest += *rest == ':';
    while (at < size && sscanf(rest, "%2x", &byte) == 1) {
        bytes[at] = (unsigned char)byte;
        at++;
        rest += 2;
    }
}

unsigned char *test_make_image(char **words, int count, size_t *len)
{
    unsigned char *bytes = nullptr;

    *len = strtoul(words[0] + 1, nullptr, 16);
    bytes = calloc(*len + 1, 1);
    for (int i = 1; bytes && i < count; i++)
        put_patch(bytes, *len, words[i]);
    return (bytes);
}
