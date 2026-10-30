/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static char *copy_text(const char *text, size_t size)
{
    char *copy = malloc(size + 1);

    if (copy)
        memcpy(copy, text, size);
    if (copy)
        copy[size] = 0;
    return (copy);
}

bool read_own(const handed_t *of, const char *key, value_t *v)
{
    size_t size = 0;
    const char *raw = manifest_get_value((const unsigned char *)of->bytes,
        of->len, key, &size);
    char *text = raw ? malloc(size + 8) : nullptr;
    toml_t t = {};

    if (!text)
        return (false);
    snprintf(text, size + 8, "v = %.*s\n", (int)size, raw);
    if (toml_read(text, &t) && t.count == 1) {
        *v = t.entries[0].value;
        t.entries[0].value = (value_t){};
    } else {
        *v = (value_t){ .kind = VALUE_TEXT, .text = copy_text(raw, size) };
    }
    toml_free(&t);
    free(text);
    return (v->kind != VALUE_TEXT || v->text);
}

const value_t *state_find_own(const stating_t *s, const module_rule_t *rule)
{
    value_t *own = state_keep_value(s, calloc(1, sizeof(*own)));
    const char *why = nullptr;

    if (!own || !read_own(s->of, rule->name, own))
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
    if (!who->has_pinned)
        say("%sits file states no pinned\n", s->prefix);
    if (!who->has_reloadable)
        say("%sits file states no reloadable\n", s->prefix);
    say_own_keys(s);
    return (who->config[0] && who->has_pinned && who->has_reloadable);
}
