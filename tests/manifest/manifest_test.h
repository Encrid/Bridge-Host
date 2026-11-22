/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/

#ifndef MANIFEST_TEST_H
    #define MANIFEST_TEST_H
    #include <stddef.h>
    #include <stdint.h>

    #include "manifest_inside.h"

constexpr uint32_t TEST_STATED_MOST = 8;
constexpr size_t TEST_KEY_MOST = 128;
constexpr int TEST_PATH = 2;

typedef struct {
    char key[TEST_KEY_MOST];
    manifest_value_t value;
    const char *text;
    int got;
} test_stated_t;

typedef struct {
    test_stated_t stated[TEST_STATED_MOST];
    uint32_t stated_count;
    char phases[TEST_KEY_MOST];
    uint32_t undeclared;
    bool deaf;
    bool any;
    manifest_value_t rest;
} test_host_t;

typedef struct {
    manifest_host_t host;
    test_host_t told;
    manifest_made_t made;
    manifest_t who;
    manifest_module_t m;
    unsigned char *bytes;
    size_t len;
} test_module_t;

[[nodiscard]] bool test_make_value(const char *spec, manifest_value_t *v);
[[nodiscard]] bool test_parse_value(void *ud, const char *text,
    manifest_value_t *v);
void test_show_value(const manifest_value_t *v);
void test_show_setting(const bridge_setting_t *set);
void test_show_config(const bridge_module_config_t *cfg, bool taken);
void test_tell(void *ud, const manifest_told_t *told);
void test_load(test_module_t *t, char **words, int count);
[[nodiscard]] unsigned char *test_make_image(char **words, int count,
    size_t *len);
void test_set_host(test_module_t *t);
[[nodiscard]] int test_open_module(test_module_t *t, int argc, char **argv);
void test_end_module(test_module_t *t);
int test_run_read(int argc, char **argv);
int test_run_find(int argc, char **argv);
int test_run_value(int argc, char **argv);
int test_run_check(int argc, char **argv);
int test_run_rule(int argc, char **argv);
int test_run_allow(int argc, char **argv);
int test_run_setting(int argc, char **argv);
int test_run_state(int argc, char **argv);
int test_run_key(int argc, char **argv);
int test_run_names(int argc, char **argv);
int test_run_keep(int argc, char **argv);

#endif
