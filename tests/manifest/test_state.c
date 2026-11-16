/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "manifest_test.h"

int test_run_state(int argc, char **argv)
{
    test_module_t t = {};
    bridge_module_config_t cfg = {};
    int failed = test_open_module(&t, argc - 2, argv + 2);
    bool taken = false;

    if (!failed) {
        taken = manifest_state(&t.m, &cfg);
        printf("state: %s\n", taken ? "taken" : "refused");
        test_show_config(&cfg, taken);
    }
    test_end_module(&t);
    return (failed);
}

int test_run_key(int argc, char **argv)
{
    test_module_t t = {};
    const manifest_value_t *v = nullptr;
    int failed = test_open_module(&t, argc - 3, argv + 3);

    if (!failed)
        v = manifest_state_key(&t.m, argv[2]);
    printf("key: %s", v ? "" : "none");
    if (v)
        test_show_value(v);
    printf("\n");
    test_end_module(&t);
    return (failed);
}
