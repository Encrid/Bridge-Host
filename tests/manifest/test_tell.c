/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "manifest_test.h"

void test_tell(void *ud, const manifest_told_t *told)
{
    static const char *const KINDS[] = { "", "value", "not stated", "twice",
        "wrong", "no word", "no component" };

    (void)ud;
    printf("%s:", KINDS[told->kind]);
    for (uint32_t i = 0; i < told->depth; i++)
        printf("%s%s", i ? "." : " ", told->names[i]);
    if (told->value) {
        printf(" = ");
        test_show_value(told->value);
    }
    if (told->why)
        printf(" (%s)", told->why);
    printf("\n");
}
