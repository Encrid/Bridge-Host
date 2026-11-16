/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifest_test.h"

static char TEST_REST[] = "none";

static void join_names(char *key, const char *const *names, uint32_t depth)
{
    size_t at = 0;

    key[0] = '\0';
    for (uint32_t i = 0; i < depth && at < TEST_KEY_MOST; i++)
        at += (size_t)snprintf(key + at, TEST_KEY_MOST - at, "%s%s",
            i ? "." : "", names[i]);
}

static int find(void *ud, const char *const *names, uint32_t depth,
    manifest_found_t *found)
{
    test_host_t *told = ud;
    char key[TEST_KEY_MOST];

    join_names(key, names, depth);
    for (uint32_t i = 0; i < told->stated_count; i++) {
        if (strcmp(told->stated[i].key, key))
            continue;
        *found = (manifest_found_t){ &told->stated[i].value,
            told->stated[i].text };
        return (told->stated[i].got);
    }
    *found = (manifest_found_t){ &told->rest, nullptr };
    return (told->any ? 1 : 0);
}

static uint32_t phases(void *ud, const char ***names)
{
    test_host_t *told = ud;
    uint32_t count = 0;
    char *name = strtok(told->phases, ",");

    *names = calloc(TEST_STATED_MOST, sizeof(**names));
    while (*names && name && count < TEST_STATED_MOST) {
        (*names)[count] = name;
        count++;
        name = strtok(nullptr, ",");
    }
    return (*names ? count : 0);
}

static uint32_t undeclared(void *ud, const char *const *names, uint32_t count)
{
    const test_host_t *told = ud;

    (void)names;
    (void)count;
    return (told->undeclared);
}

void test_set_host(test_module_t *t)
{
    t->told.rest = (manifest_value_t){ .kind = MANIFEST_TEXT,
        .text = TEST_REST };
    t->host = (manifest_host_t){ .ud = &t->told, .parse = test_parse_value,
        .find = find, .phases = phases, .undeclared = undeclared,
        .tell = test_tell };
    t->m = (manifest_module_t){ .host = &t->host, .who = &t->who,
        .made = &t->made };
}
