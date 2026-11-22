/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifest_test.h"

int test_run_names(int argc, char **argv)
{
    manifest_names_t list = {};

    for (int i = 2; i < argc; i++)
        manifest_add_name(&list, argv[i]);
    printf("names:");
    for (uint32_t i = 0; i < list.count; i++)
        printf(" %s", list.names[i]);
    printf(", lost %d\n", list.lost);
    free(list.names);
    return (0);
}

int test_run_keep(int argc, char **argv)
{
    manifest_made_t made = {};
    bool none = argc == 3 && !strcmp(argv[2], "null");
    int asked = none ? 1 : atoi(argv[2]);
    int kept = 0;

    for (int i = 0; i < asked; i++)
        kept += manifest_keep(&made, none ? nullptr : malloc(8)) != nullptr;
    printf("keep: %d of %d kept, %u in the list\n", kept, asked,
        made.block_count);
    manifest_free(&made);
    return (0);
}
