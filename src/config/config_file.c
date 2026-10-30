/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "file_bytes.h"
#include "os.h"
#include "say.h"

static char *read_text(const char *path)
{
    size_t len = 0;
    char *text = read_whole(path, &len);
    FILE *there = text ? nullptr : fopen(path, "rb");

    if (there) {
        fclose(there);
        text = calloc(1, 1);
    }
    return (text);
}

static bool take_text(cfile_t *f, char *text)
{
    bool taken = toml_read(text, &f->toml);

    free(text);
    if (!taken)
        say("refused: %s:%u: %s\n", f->path, f->toml.line, f->toml.why);
    return (taken);
}

int32_t config_add_file(config_t *c, char *path, int32_t by, uint32_t line)
{
    char *text = read_text(path);
    cfile_t *more = text ? realloc(c->files, sizeof(*more) * (c->count + 1))
        : nullptr;
    cfile_t *f = nullptr;

    if (!more) {
        say("cannot read '%s'\n", path);
        free(text);
        free(path);
        return (-1);
    }
    c->files = more;
    f = &c->files[c->count];
    *f = (cfile_t){ .path = path, .folder = config_get_folder(path),
        .whole = os_path_whole(path), .by = by, .by_line = line };
    if (take_text(f, text))
        return ((int32_t)c->count++);
    config_free_file(f);
    return (-1);
}

int32_t config_find_whole(const config_t *c, const char *whole)
{
    for (uint32_t i = 0; i < c->count; i++)
        if (whole && c->files[i].whole && !strcmp(c->files[i].whole, whole))
            return ((int32_t)i);
    return (-1);
}

void config_say_chain(const config_t *c, int32_t from, int32_t to)
{
    if (from != to && from >= 0)
        config_say_chain(c, c->files[from].by, to);
    say_part(" %s", c->files[from].path);
}
