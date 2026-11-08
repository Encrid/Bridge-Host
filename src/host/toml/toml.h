/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_TOML_H
    #define BRIDGE_HOST_TOML_H

    #include <stddef.h>
    #include <stdint.h>

    #include "bridge_host_manifest.h"

typedef manifest_value_t value_t;

typedef struct {
    char **key;
    uint32_t depth;
    uint32_t line;
    value_t value;
} entry_t;

typedef struct {
    entry_t *entries;
    uint32_t count;
    char **names;
    uint32_t name_count;
    uint32_t line;
    const char *why;
} toml_t;

typedef struct {
    const char *at;
    uint32_t line;
    uint32_t start;
    char **table;
    uint32_t table_depth;
    toml_t *into;
} reading_t;

[[nodiscard]] bool toml_read(const char *text, toml_t *t);

bool toml_refuse(reading_t *r, const char *why);

void toml_skip(reading_t *r);

[[nodiscard]] bool toml_read_pair(reading_t *r);

[[nodiscard]] bool toml_read_inline(reading_t *r, char **key,
    uint32_t depth);

[[nodiscard]] bool toml_read_key(reading_t *r, char ***key, uint32_t *depth);

[[nodiscard]] bool toml_read_value(reading_t *r, value_t *v);

[[nodiscard]] char *toml_read_text(reading_t *r);

[[nodiscard]] bool toml_read_number(reading_t *r, value_t *v);

[[nodiscard]] bool toml_add(reading_t *r, char **key, uint32_t depth,
    const value_t *v);

[[nodiscard]] bool toml_has_prefix(const entry_t *e, const char *const *names,
    uint32_t depth);

[[nodiscard]] bool toml_is_same(const value_t *a, const value_t *b);

void toml_say(const value_t *v);

void toml_free(toml_t *t);

#endif
