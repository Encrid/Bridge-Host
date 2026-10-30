/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool parse_own(const char *raw, value_t *v)
{
    size_t size = strlen(raw) + sizeof("v = \n");
    char *text = calloc(size, 1);
    toml_t t = {};

    if (!text)
        return (false);
    snprintf(text, size, "v = %s\n", raw);
    if (toml_read(text, &t) && t.count == 1) {
        *v = t.entries[0].value;
        t.entries[0].value = (value_t){};
        free(text);
    } else {
        *v = (value_t){ .kind = VALUE_TEXT, .text = text };
        memcpy(text, raw, strlen(raw) + 1);
    }
    toml_free(&t);
    return (true);
}

bool read_own(const stating_t *s, const char *key, value_t *v)
{
    const char *raw = nullptr;
    uint32_t count = manifest_get_value((const unsigned char *)s->of->bytes,
        s->of->len, key, &raw);

    if (count > 1)
        say("%sits file states %s twice\n", s->prefix, key);
    return (count == 1 && parse_own(raw, v));
}

const value_t *state_find_own(const stating_t *s, const module_rule_t *rule)
{
    value_t *own = state_keep_value(s, calloc(1, sizeof(*own)));
    const char *why = nullptr;

    if (!own || !read_own(s, rule->name, own))
        return (nullptr);
    why = config_check_value(own, rule->check);
    if (why)
        say("%sits file states %s: %s\n", s->prefix, rule->name, why);
    return (why ? nullptr : own);
}

static void say_own_keys(const stating_t *s)
{
    const manifest_t *who = &s->of->who;

    if (!s->show)
        return;
    say("  config = \"%s\"  its file\n", who->config);
    say("  settings = \"%s\"  its file\n", who->settings);
    say("  pinned = %s  its file\n", who->pinned ? "true" : "false");
    say("  reloadable = %s  its file\n",
        who->reloadable ? "true" : "false");
}

bool state_own_keys(const stating_t *s)
{
    const manifest_t *who = &s->of->who;

    if (!who->config[0])
        say("%sits file states no config\n", s->prefix);
    if (!(who->taken & MANIFEST_PINNED))
        say("%sits file states no pinned\n", s->prefix);
    if (!(who->taken & MANIFEST_RELOADABLE))
        say("%sits file states no reloadable\n", s->prefix);
    say_own_keys(s);
    return (who->config[0] && (who->taken & MANIFEST_PINNED)
        && (who->taken & MANIFEST_RELOADABLE));
}
