/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "toml.h"

void toml_free_value(value_t *v)
{
    for (uint32_t i = 0; i < v->count; i++)
        toml_free_value(&v->items[i]);
    free(v->items);
    free(v->text);
}

void toml_free(toml_t *t)
{
    for (uint32_t i = 0; i < t->count; i++) {
        free(t->entries[i].key);
        toml_free_value(&t->entries[i].value);
    }
    for (uint32_t i = 0; i < t->name_count; i++)
        free(t->names[i]);
    free(t->entries);
    free(t->names);
}
