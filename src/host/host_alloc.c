/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdckdint.h>
#include <stdint.h>
#include <stdlib.h>

#include "host_alloc.h"
#include "say.h"

static void *said(void *block, size_t bytes)
{
    if (!block && bytes)
        say("no memory left for %zu bytes\n", bytes);
    return (block);
}

void *host_malloc(size_t bytes)
{
    return (said(malloc(bytes), bytes));
}

void *host_calloc(size_t count, size_t size)
{
    size_t bytes = 0;

    if (ckd_mul(&bytes, count, size))
        bytes = SIZE_MAX;
    return (said(calloc(count, size), bytes));
}

void *host_realloc(void *block, size_t bytes)
{
    return (said(realloc(block, bytes), bytes));
}
