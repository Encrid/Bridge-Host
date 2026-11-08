/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_MANIFEST_H
    #define BRIDGE_HOST_MANIFEST_H

    #include <stddef.h>
    #include <stdint.h>

    #include "bridge.h"

enum manifest_value_kind : uint8_t {
    MANIFEST_TEXT = 1,
    MANIFEST_WHOLE,
    MANIFEST_REAL,
    MANIFEST_FLAG,
    MANIFEST_LIST,
    MANIFEST_TABLE
};

typedef struct manifest_value manifest_value_t;

struct manifest_value {
    enum manifest_value_kind kind;
    bool flag;
    int64_t whole;
    double real;
    char *text;
    manifest_value_t *items;
    uint32_t count;
};

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

enum manifest_allow : uint8_t {
    MANIFEST_ALLOW_FLAG = 1,
    MANIFEST_ALLOW_WHOLE = 2,
    MANIFEST_ALLOW_REAL = 4,
    MANIFEST_ALLOW_WORD = 8,
    MANIFEST_ALLOW_TEXT = 16,
    MANIFEST_ALLOW_EMPTY = 32,
    MANIFEST_ALLOW_NAMES = 64,
    MANIFEST_ALLOW_PAIRS = 128
};

enum manifest_check : uint8_t {
    MANIFEST_CHECK_FLAG = 1,
    MANIFEST_CHECK_COUNT,
    MANIFEST_CHECK_LIMIT,
    MANIFEST_CHECK_LEVEL,
    MANIFEST_CHECK_NAMES,
    MANIFEST_CHECK_NAMES_NONE,
    MANIFEST_CHECK_ERROR,
    MANIFEST_CHECK_TEXT,
    MANIFEST_CHECK_SETTING,
    MANIFEST_CHECK_AUTHOR
};

typedef struct {
    uint8_t allow;
    int64_t least;
    int64_t most;
    const char *words;
    const char *why;
} manifest_rule_t;

enum manifest_told_kind : uint8_t {
    MANIFEST_TOLD_VALUE = 1,
    MANIFEST_TOLD_NOT_STATED,
    MANIFEST_TOLD_TWICE,
    MANIFEST_TOLD_WRONG,
    MANIFEST_TOLD_NO_WORD,
    MANIFEST_TOLD_NO_COMPONENT
};

typedef struct {
    enum manifest_told_kind kind;
    const char *const *names;
    uint32_t depth;
    const manifest_value_t *value;
    const char *why;
} manifest_told_t;

typedef struct {
    const manifest_value_t *value;
    const char *text;
} manifest_found_t;

typedef struct {
    void *ud;
    bool (*parse)(void *ud, const char *text, manifest_value_t *v);
    int (*find)(void *ud, const char *const *names, uint32_t depth,
        manifest_found_t *found);
    uint32_t (*phases)(void *ud, const char ***names);
    uint32_t (*undeclared)(void *ud, const char *const *names,
        uint32_t count);
    void (*tell)(void *ud, const manifest_told_t *told);
} manifest_host_t;

typedef struct {
    void **blocks;
    uint32_t block_count;
    manifest_value_t **values;
    uint32_t value_count;
} manifest_made_t;

typedef struct {
    const manifest_host_t *host;
    const manifest_t *who;
    const unsigned char *bytes;
    size_t len;
    manifest_made_t *made;
} manifest_module_t;

[[nodiscard]] const char *manifest_read(const unsigned char *bytes,
    size_t len, manifest_t *m);

[[nodiscard]] uint32_t manifest_get_value(const unsigned char *bytes,
    size_t len, const char *key, const char **value);

[[nodiscard]] const char *manifest_check_value(const manifest_value_t *v,
    const manifest_rule_t *rule);

[[nodiscard]] const char *manifest_check_key(const char *name,
    const manifest_value_t *v);

[[nodiscard]] const manifest_rule_t *manifest_get_rule(
    enum manifest_check check);

[[nodiscard]] bool manifest_state(const manifest_module_t *m,
    bridge_module_config_t *cfg);

[[nodiscard]] const manifest_value_t *manifest_state_key(
    const manifest_module_t *m, const char *name);

void manifest_free(manifest_made_t *made);

void manifest_free_value(manifest_value_t *v);

#endif
