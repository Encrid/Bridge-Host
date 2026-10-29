/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_MANIFEST_H
    #define BRIDGE_HOST_MANIFEST_H

    #include <stddef.h>
    #include <stdint.h>

typedef struct {
    const char *identity;
    const char *backend;
    const char *config;
    const char *settings;
    int64_t abi;
    bool pinned;
    bool reloadable;
    bool has_pinned;
    bool has_reloadable;
} manifest_t;

[[nodiscard]] const char *manifest_read(const unsigned char *bytes, size_t len,
    manifest_t *m);

[[nodiscard]] const char *manifest_get_value(const unsigned char *bytes,
    size_t len, const char *key, size_t *size);

#endif
