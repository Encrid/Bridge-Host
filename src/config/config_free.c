/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "config.h"

void config_free_file(cfile_t *f)
{
    free(f->path);
    free(f->folder);
    free(f->whole);
    free(f->named);
    toml_free(&f->toml);
}

void config_free(config_t *c)
{
    for (uint32_t i = 0; i < c->count; i++)
        config_free_file(&c->files[i]);
    free(c->files);
}
