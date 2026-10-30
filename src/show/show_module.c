/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

void show_module(void *ud, const char *path, os_folder_t *in,
    const char *name)
{
    host_t *h = ud;
    handed_t now = {};
    made_t made = {};
    const stating_t s = { h, &now, "  refused: ", true, &made };
    const char *why = nullptr;

    now.bytes = os_folder_read_file(in, name, &now.len);
    if (!now.bytes) {
        say("cannot read '%s'\n", path);
        return;
    }
    why = manifest_read((const unsigned char *)now.bytes, now.len, &now.who);
    if (why)
        say("not a module: %s: %s\n", path, why);
    else
        say("module %s, %s, %s\n", now.who.identity, now.who.backend, path);
    if (!why && (config_load(h, now.who.identity, s.prefix) < 0
        || !state_found(&s, &(bridge_module_config_t){})))
        h->counts.failed++;
    state_free(&made);
}
