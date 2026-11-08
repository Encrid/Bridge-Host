/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_MANIFEST_INSIDE_H
    #define BRIDGE_HOST_MANIFEST_INSIDE_H

    #include "bridge_host_manifest.h"

typedef bool (*finder_t)(const unsigned char *bytes, size_t len, size_t *at,
    size_t *size);

typedef struct {
    const char *first;
    size_t count;
    finder_t find;
} format_t;

typedef struct {
    const char *name;
    enum manifest_check check;
    uint8_t width;
    size_t at;
    int64_t none;
} key_rule_t;

typedef struct {
    const char *const *names;
    uint32_t depth;
} key_path_t;

typedef struct {
    const char **names;
    uint32_t count;
} name_list_t;

[[nodiscard]] uint64_t manifest_number(const unsigned char *bytes, size_t len,
    size_t at, int width);

[[nodiscard]] bool manifest_find_elf(const unsigned char *bytes, size_t len,
    size_t *at, size_t *size);

[[nodiscard]] bool manifest_find_macho(const unsigned char *bytes,
    size_t len, size_t *at, size_t *size);

[[nodiscard]] bool manifest_find_wasm(const unsigned char *bytes, size_t len,
    size_t *at, size_t *size);

[[nodiscard]] const char *manifest_find(const unsigned char *bytes, size_t len,
    size_t *at, size_t *size);

[[nodiscard]] int manifest_take(manifest_t *m, const char *key, size_t len,
    const char *value);

[[nodiscard]] const char *manifest_read_word(manifest_t *m, const char *word,
    bool first);

[[nodiscard]] const key_rule_t *manifest_get_key(uint32_t at);

[[nodiscard]] const key_rule_t *manifest_find_key(const char *name);

void manifest_put(bridge_module_config_t *cfg, const key_rule_t *rule,
    const manifest_value_t *v);

[[nodiscard]] void *manifest_keep(const manifest_module_t *m, void *block);

[[nodiscard]] manifest_value_t *manifest_keep_value(
    const manifest_module_t *m, manifest_value_t *v);

void manifest_tell(const manifest_module_t *m, enum manifest_told_kind kind,
    const key_path_t *path, const manifest_value_t *v);

[[nodiscard]] const manifest_value_t *manifest_find_own(
    const manifest_module_t *m, const key_path_t *path,
    enum manifest_check check);

[[nodiscard]] bool manifest_own_words(const manifest_module_t *m);

[[nodiscard]] const manifest_value_t *manifest_find_value(
    const manifest_module_t *m, const key_path_t *path,
    enum manifest_check check, manifest_found_t *found);

void manifest_add_name(name_list_t *into, const char *name);

[[nodiscard]] bool manifest_take_list(const manifest_module_t *m,
    const key_rule_t *rule, bridge_module_config_t *cfg);

[[nodiscard]] bool manifest_take_phases(const manifest_module_t *m,
    bridge_module_config_t *cfg);

[[nodiscard]] bool manifest_take_settings(const manifest_module_t *m,
    bridge_module_config_t *cfg);

#endif
