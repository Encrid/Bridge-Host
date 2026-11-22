/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "manifest_test.h"
#include "rows.h"

typedef struct {
    const char *name;
    int count;
    int (*run)(int argc, char **argv);
} test_mode_t;

static const test_mode_t MODES[] = {
    { "read", 2, test_run_read }, { "find", 3, test_run_find },
    { "value", 3, test_run_value }, { "check", 4, test_run_check },
    { "rule", 4, test_run_rule }, { "allow", 4, test_run_allow },
    { "setting", 4, test_run_setting },
    { "state", 2, test_run_state }, { "key", 3, test_run_key },
    { "names", 2, test_run_names }, { "keep", 3, test_run_keep }
};

static int run_mode(int argc, char **argv)
{
    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name) && argc >= MODES[i].count)
            return (MODES[i].run(argc, argv));
    printf("usage: manifest-unit <mode> [arguments]\n");
    return (2);
}

static int sweep_mode(int argc, char **argv)
{
    unsigned at = 0;
    int status = 0;

    do {
        at++;
        sweep_refuse(argv[2], at);
        status = run_mode(argc - 2, argv + 2);
        if (sweep_is_refused())
            printf("-- %s %u refused, status %d\n", argv[2], at, status);
    } while (sweep_is_refused());
    sweep_refuse(argv[2], 0);
    return (status);
}

static int run(int argc, char **argv)
{
    if (argc > 3 && !strcmp(argv[1], "sweep"))
        return (sweep_mode(argc, argv));
    return (run_mode(argc, argv));
}

int main(int argc, char **argv)
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    return (argc > 1 ? run(argc, argv) : rows_run(run));
}
