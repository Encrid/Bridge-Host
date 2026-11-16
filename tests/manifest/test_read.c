/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "manifest_test.h"

static void find_one(const manifest_module_t *m, char *key)
{
    char *dot = strchr(key, '.');
    const char *names[2] = { key, dot ? dot + 1 : nullptr };
    const key_path_t path = { names, dot ? 2 : 1 };
    const key_rule_t *rule = nullptr;
    const manifest_value_t *v = nullptr;

    if (dot)
        *dot = '\0';
    rule = manifest_find_key(key);
    v = manifest_find_own(m, &path, rule ? rule->check : MANIFEST_CHECK_FLAG);
    printf("%s: %s", v ? "found" : "none", names[path.depth - 1]);
    if (v) {
        printf(" = ");
        test_show_value(v);
    }
    printf("\n");
}

int test_run_read(int argc, char **argv)
{
    test_module_t t = {};
    int failed = test_open_module(&t, argc - 2, argv + 2);

    if (!failed) {
        printf("who: %s backend %s abi %lld\n", t.who.identity,
            t.who.backend, (long long)t.who.abi);
        printf("words: config '%s' settings '%s' pinned %d reloadable %d\n",
            t.who.config, t.who.settings, t.who.pinned, t.who.reloadable);
    }
    test_end_module(&t);
    return (failed);
}

int test_run_find(int argc, char **argv)
{
    test_module_t t = {};
    int failed = test_open_module(&t, argc - 3, argv + 3);

    if (!failed)
        find_one(&t.m, argv[2]);
    test_end_module(&t);
    return (failed);
}

int test_run_value(int argc, char **argv)
{
    test_module_t t = {};
    const char *value = "";
    uint32_t count = 0;

    test_set_host(&t);
    test_load(&t, argv + 3, argc - 3);
    if (t.bytes)
        count = manifest_get_value(t.bytes, t.len, argv[2], &value);
    printf("value: %u '%s'\n", count, count ? value : "");
    test_end_module(&t);
    return (0);
}
