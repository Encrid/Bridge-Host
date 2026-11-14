/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifest_inside.h"

static bool parse(void *ud, const char *text, manifest_value_t *v)
{
    (void)ud;
    if (!strcmp(text, "true") || !strcmp(text, "false"))
        *v = (manifest_value_t){ .kind = MANIFEST_FLAG,
            .flag = text[0] == 't' };
    else
        *v = (manifest_value_t){ .kind = MANIFEST_WHOLE,
            .whole = strtoll(text, nullptr, 10) };
    return (true);
}

static void write_told(void *ud, const manifest_told_t *told)
{
    static const char *const KINDS[] = { "", "value", "not stated", "twice",
        "wrong", "no word", "no component" };

    (void)ud;
    printf("%s: %s\n", KINDS[told->kind], told->names[told->depth - 1]);
}

static unsigned char *read_file(const char *path, size_t *len)
{
    FILE *file = fopen(path, "rb");
    unsigned char *bytes = nullptr;
    long size = 0;

    if (!file)
        return (nullptr);
    if (!fseek(file, 0, SEEK_END))
        size = ftell(file);
    rewind(file);
    if (size > 0)
        bytes = malloc((size_t)size);
    if (bytes)
        *len = fread(bytes, 1, (size_t)size, file);
    fclose(file);
    return (bytes);
}

static void find_one(const manifest_module_t *m, char *key)
{
    char *dot = strchr(key, '.');
    const char *names[2] = { key, dot ? dot + 1 : nullptr };
    const key_path_t path = { names, dot ? 2 : 1 };
    const key_rule_t *rule = nullptr;
    const manifest_value_t *v = nullptr;

    if (dot)
        *dot = '\0';
    rule = manifest_find_key(key);
    v = manifest_find_own(m, &path, rule ? rule->check : MANIFEST_CHECK_FLAG);
    printf("%s: %s\n", v ? "found" : "none", names[path.depth - 1]);
}

int main(int argc, char **argv)
{
    const manifest_host_t host = { .parse = parse, .tell = write_told };
    manifest_made_t made = {};
    manifest_t who = {};
    size_t len = 0;
    unsigned char *bytes = argc > 1 ? read_file(argv[1], &len) : nullptr;
    const manifest_module_t m = { &host, &who, bytes, len, &made };
    const char *why = bytes ? manifest_read(bytes, len, &who) : "no file";

    if (why)
        printf("no manifest: %s\n", why);
    for (int i = 2; !why && i < argc; i++)
        find_one(&m, argv[i]);
    manifest_free(&made);
    free(bytes);
    return (why != nullptr);
}
