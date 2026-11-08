/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_MANIFEST_H
    #define BRIDGE_HOST_MANIFEST_H

    #include <stddef.h>
    #include <stdint.h>

enum manifest_word : uint8_t {
    MANIFEST_ABI = 1,
    MANIFEST_CONFIG = 2,
    MANIFEST_SETTINGS = 4,
    MANIFEST_PINNED = 8,
    MANIFEST_RELOADABLE = 16
};

typedef struct {
    const char *identity;
    const char *backend;
    const char *config;
    const char *settings;
    int64_t abi;
    bool pinned;
    bool reloadable;
    uint8_t taken;
} manifest_t;

[[nodiscard]] const char *manifest_read(const unsigned char *bytes, size_t len,
    manifest_t *m);

[[nodiscard]] uint32_t manifest_get_value(const unsigned char *bytes,
    size_t len, const char *key, const char **value);

#endif
