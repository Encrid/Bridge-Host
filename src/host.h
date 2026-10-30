/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_HOST_H
    #define BRIDGE_HOST_HOST_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "bridge.h"
    #include "bridge_abi.h"
    #include "config.h"
    #include "file_bytes.h"
    #include "manifest.h"
    #include "say.h"

enum arrive : uint8_t {
    ARRIVE_UNSTATED,
    ARRIVE_ASK,
    ARRIVE_LOAD,
    ARRIVE_LEAVE
};

enum replaced : uint8_t {
    REPLACED,
    REPLACE_REFUSED,
    REPLACE_NOT_HOT,
    REPLACE_KEPT
};

typedef struct {
    int failed;
    int said;
} counts_t;

typedef struct {
    manifest_t who;
    char *bytes;
    size_t len;
} handed_t;

typedef struct {
    char *path;
    uint64_t mark;
    handed_t *of;
    bool loaded;
    bool seen;
} file_t;

typedef struct {
    const char *name;
    char *library;
    enum arrive start;
    enum arrive arrive;
    bridge_setting_t *settings;
} held_t;

typedef struct {
    config_t config;
    deployment_t d;
    const char **phases;
    uint32_t phase_count;
    bool ended;
    int round;
    held_t *held;
    bridge_backend_t *backends;
    uint32_t backend_count;
    bridge_order_t *order;
    uint32_t order_count;
    char **paths;
    uint32_t path_count;
    char **ignore;
    uint32_t ignore_count;
    bridge_world_t *w;
    file_t *files;
    file_t **found;
    bool looked;
    bool full;
    counts_t counts;
} host_t;

typedef struct {
    void **blocks;
    uint32_t block_count;
    void **values;
    uint32_t value_count;
} made_t;

typedef struct {
    const host_t *h;
    const handed_t *of;
    const char *prefix;
    bool show;
    made_t *made;
} stating_t;

typedef struct {
    file_t **found;
    int count;
    int did;
    bool full;
} look_t;

typedef struct {
    host_t *h;
    look_t *lk;
} seeing_t;

typedef void see_fn(void *ud, const char *path);

typedef struct {
    const host_t *h;
    see_fn *see;
    void *ud;
} walk_t;

[[nodiscard]] bool config_world(host_t *h);

[[nodiscard]] bool config_order(host_t *h);

[[nodiscard]] bool config_backends(host_t *h);

[[nodiscard]] bool config_backend_settings(host_t *h, uint32_t at);

[[nodiscard]] const held_t *get_held(const host_t *h, const char *name);

[[nodiscard]] bool config_content(host_t *h);

[[nodiscard]] int config_load(const host_t *h, const char *identity,
    const char *prefix);

[[nodiscard]] uint32_t config_unfound(const host_t *h);

[[nodiscard]] bool state_found(const stating_t *s,
    bridge_module_config_t *cfg);

[[nodiscard]] bool read_own(const stating_t *s, const char *key, value_t *v);

[[nodiscard]] const value_t *state_find_own(const stating_t *s,
    const key_rule_t *rule);

[[nodiscard]] bool state_own_keys(const stating_t *s);

[[nodiscard]] const value_t *state_find_key(const stating_t *s,
    const key_rule_t *rule);

[[nodiscard]] bool state_take_key(const stating_t *s,
    const key_rule_t *rule, bridge_module_config_t *cfg);

[[nodiscard]] bool state_take_list(const stating_t *s,
    const key_rule_t *rule, bridge_module_config_t *cfg);

[[nodiscard]] bool state_take_phases(const stating_t *s,
    bridge_module_config_t *cfg);

[[nodiscard]] bool state_settings(const stating_t *s,
    bridge_module_config_t *cfg);

[[nodiscard]] bool state_is_setting(const entry_t *e,
    const char *const *two);

[[nodiscard]] bool state_states_setting(const cfile_t *f, const void *asked,
    said_t *said);

[[nodiscard]] bool state_is_path(const config_t *c, const said_t *said);

[[nodiscard]] said_t state_own_setting(const stating_t *s, const char *key);

[[nodiscard]] uint32_t state_undeclared(const stating_t *s,
    const names_t *declared);

[[nodiscard]] const char *state_keep_prefix(made_t *m, const char *what,
    const char *path, const char *who);

[[nodiscard]] void *state_keep(const stating_t *s, void *block);

[[nodiscard]] value_t *state_keep_value(const stating_t *s, value_t *v);

void state_free(made_t *m);

[[nodiscard]] int walk_content(const host_t *h, see_fn *see, void *ud);

[[nodiscard]] int show_config(host_t *h, bool taken);

void show_module(void *ud, const char *path);

[[nodiscard]] bool take_lock(const char *name);

void host_free(host_t *h);

void handed_free(handed_t *of);

[[nodiscard]] int run(host_t *h);

[[nodiscard]] int look(host_t *h, bool first);

[[nodiscard]] file_t *find_file(host_t *h, const char *path);

[[nodiscard]] bool meet(host_t *h, file_t *f, const char *path,
    handed_t *now);

[[nodiscard]] bool may_arrive(const file_t *f, const held_t *held,
    enum arrive rule);

void load_found(host_t *h, file_t *f, bool first);

[[nodiscard]] bool make_artifact(const stating_t *s, const file_t *f,
    bridge_artifact_t *a);

#endif
