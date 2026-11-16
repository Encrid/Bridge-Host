/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "manifest_test.h"

int test_run_check(int argc, char **argv)
{
    manifest_value_t v = {};
    const char *why = "no value";

    (void)argc;
    if (test_make_value(argv[3], &v))
        why = manifest_check_key(argv[2], &v);
    printf("check: %s\n", why ? why : "allowed");
    manifest_free_value(&v);
    return (0);
}

int test_run_rule(int argc, char **argv)
{
    enum manifest_check check = (enum manifest_check)atoi(argv[2]);
    manifest_value_t v = {};
    const char *why = "no value";

    (void)argc;
    if (test_make_value(argv[3], &v))
        why = manifest_check_value(&v, manifest_get_rule(check));
    printf("rule: %s\n", why ? why : "allowed");
    manifest_free_value(&v);
    return (0);
}

int test_run_setting(int argc, char **argv)
{
    manifest_value_t v = {};
    bridge_setting_t set = {};

    if (!test_make_value(argv[3], &v))
        return (1);
    set = manifest_get_setting(argv[2], &v, argc > 4 ? argv[4] : v.text);
    printf("setting: kind %d", set.kind);
    test_show_setting(&set);
    printf("\n");
    manifest_free_value(&v);
    return (0);
}

int test_run_allow(int argc, char **argv)
{
    const manifest_rule_t rule = { (uint8_t)atoi(argv[2]), 1, 10, "one two ",
        "refused" };
    manifest_value_t v = {};
    const char *why = "no value";

    (void)argc;
    if (test_make_value(argv[3], &v))
        why = manifest_check_value(&v, &rule);
    printf("allow: %s\n", why ? why : "allowed");
    manifest_free_value(&v);
    return (0);
}
