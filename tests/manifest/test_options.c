/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifest_test.h"

static void add_stated(test_host_t *told, char *word, int got)
{
    char *spec = strchr(word, '=');
    test_stated_t *one = &told->stated[told->stated_count];

    if (told->stated_count == TEST_STATED_MOST)
        return;
    told->stated_count++;
    snprintf(one->key, sizeof(one->key), "%.*s",
        (int)(spec ? spec - word : (ptrdiff_t)strlen(word)), word);
    one->got = got;
    if (!spec)
        return;
    if (got == TEST_PATH) {
        one->value = (manifest_value_t){ .kind = MANIFEST_TABLE };
        one->text = spec + 1;
        one->got = 1;
    } else if (!test_make_value(spec + 1, &one->value)) {
        one->got = -1;
    }
}

static bool take_option(test_host_t *told, char *word)
{
    if (!strncmp(word, "find:", 5))
        add_stated(told, word + 5, 1);
    else if (!strncmp(word, "wrong:", 6))
        add_stated(told, word + 6, -1);
    else if (!strncmp(word, "path:", 5))
        add_stated(told, word + 5, TEST_PATH);
    else if (!strncmp(word, "phases:", 7))
        snprintf(told->phases, sizeof(told->phases), "%s", word + 7);
    else if (!strncmp(word, "undeclared:", 11))
        told->undeclared = (uint32_t)strtoul(word + 11, nullptr, 10);
    else if (!strncmp(word, "own:", 4))
        add_stated(told, word + 4, 0);
    else if (!strcmp(word, "any"))
        told->any = true;
    else if (!strcmp(word, "deaf"))
        told->deaf = true;
    else
        return (false);
    return (true);
}

int test_open_module(test_module_t *t, int argc, char **argv)
{
    const char *why = "no file";
    int at = 0;

    test_set_host(t);
    while (at < argc && take_option(&t->told, argv[at]))
        at++;
    test_load(t, argv + at, argc - at);
    if (t->bytes)
        why = manifest_read(t->bytes, t->len, &t->who);
    if (why)
        printf("no manifest: %s\n", why);
    if (why && t->who.backend)
        printf("backend: %s\n", t->who.backend);
    return (why != nullptr);
}

void test_end_module(test_module_t *t)
{
    manifest_free(&t->made);
    free(t->bytes);
    for (uint32_t i = 0; i < t->told.stated_count; i++)
        manifest_free_value(&t->told.stated[i].value);
}
