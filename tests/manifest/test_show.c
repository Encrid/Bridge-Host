/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "manifest_test.h"

void test_show_value(const manifest_value_t *v)
{
    if (v->kind == MANIFEST_FLAG)
        printf("%s", v->flag ? "true" : "false");
    if (v->kind == MANIFEST_WHOLE)
        printf("%lld", (long long)v->whole);
    if (v->kind == MANIFEST_REAL)
        printf("%g", v->real);
    if (v->kind == MANIFEST_TEXT)
        printf("'%s'", v->text);
    if (v->kind == MANIFEST_TABLE)
        printf("{}");
    for (uint32_t i = 0; v->kind == MANIFEST_LIST && i < v->count; i++) {
        printf("%s", i ? "," : "[");
        test_show_value(&v->items[i]);
    }
    if (v->kind == MANIFEST_LIST)
        printf("%s", v->count ? "]" : "[]");
}

void test_show_setting(const bridge_setting_t *set)
{
    printf(" %s=", set->key);
    if (set->kind == BRIDGE_INT)
        printf("%lld", (long long)set->v.i);
    if (set->kind == BRIDGE_FLOAT)
        printf("%g", set->v.f);
    if (set->kind == BRIDGE_BOOL)
        printf("%s", set->v.b ? "true" : "false");
    if (set->kind == BRIDGE_STRING)
        printf("'%s'", set->v.s);
}

static void show_names(const char *what, const char *const *names,
    uint32_t count)
{
    printf("%s:", what);
    for (uint32_t i = 0; i < count; i++)
        printf(" %s", names[i]);
    printf("\n");
}

static void show_numbers(const bridge_module_config_t *cfg)
{
    printf("who: %s abi %u config '%s' pinned %d reloadable %d\n",
        cfg->identity, cfg->abi, cfg->config, cfg->pinned, cfg->reloadable);
    printf("flags: hot_reload %d on_error %d log_level %d\n",
        cfg->hot_reload, cfg->on_error, cfg->log_level);
    printf("limits: %lld %lld %lld %lld %lld %lld %lld\n",
        (long long)cfg->element_limit, (long long)cfg->max_system_failures,
        (long long)cfg->work_limit, (long long)cfg->memory_limit,
        (long long)cfg->init_timeout, (long long)cfg->reload_timeout,
        (long long)cfg->shutdown_timeout);
    printf("room: %u %u %u %lld %lld %lld\n", cfg->scratch, cfg->log_ring,
        cfg->spans, (long long)cfg->spawns, (long long)cfg->persistent,
        (long long)cfg->changes);
}

void test_show_config(const bridge_module_config_t *cfg, bool taken)
{
    uint32_t losses = cfg->loss_count > 0 ? (uint32_t)cfg->loss_count : 0;

    show_numbers(cfg);
    show_names("read", cfg->read, cfg->read_count);
    show_names("write", cfg->write, cfg->write_count);
    printf("loss count: %lld\n", (long long)cfg->loss_count);
    show_names("losses", cfg->losses, losses);
    printf("phases:");
    for (uint32_t i = 0; taken && i < cfg->phase_count; i++)
        printf(" %s>%s", cfg->phases[i].from, cfg->phases[i].to);
    printf("\nsettings:");
    for (uint32_t i = 0; cfg->settings && i < cfg->setting_count; i++)
        test_show_setting(&cfg->settings[i]);
    printf("\n");
}
