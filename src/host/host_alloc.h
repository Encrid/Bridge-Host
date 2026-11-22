/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_ALLOC_H
    #define BRIDGE_HOST_ALLOC_H

    #include <stddef.h>

[[nodiscard]] void *host_malloc(size_t bytes);
[[nodiscard]] void *host_calloc(size_t count, size_t size);
[[nodiscard]] void *host_realloc(void *block, size_t bytes);

#endif
