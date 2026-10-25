/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool add_one(void ***list, uint32_t *count, void *one)
{
    void **more = realloc(*list, sizeof(*more) * (*count + 1));

    if (!more)
        return (false);
    *list = more;
    (*list)[(*count)++] = one;
    return (true);
}

void *state_keep(const stating_t *s, void *block)
{
    made_t *m = s->made;

    if (block && !add_one(&m->blocks, &m->block_count, block)) {
        free(block);
        return (nullptr);
    }
    return (block);
}

value_t *state_keep_value(const stating_t *s, value_t *v)
{
    made_t *m = s->made;

    if (v && !add_one(&m->values, &m->value_count, v)) {
        free(v);
        return (nullptr);
    }
    return (v);
}

void state_free(made_t *m)
{
    for (uint32_t i = 0; i < m->block_count; i++)
        free(m->blocks[i]);
    for (uint32_t i = 0; i < m->value_count; i++) {
        toml_free_value(m->values[i]);
        free(m->values[i]);
    }
    free(m->blocks);
    free(m->values);
}
