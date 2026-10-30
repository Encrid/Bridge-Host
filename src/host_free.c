/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void free_files(host_t *h)
{
    for (uint32_t i = 0; h->files && i < h->d.files; i++) {
        free(h->files[i].path);
        free(h->files[i].of);
    }
    free(h->files);
    free(h->found);
}

void host_free(host_t *h)
{
    for (uint32_t i = 0; h->held && i < h->backend_count; i++) {
        free(h->held[i].library);
        free(h->held[i].settings);
    }
    for (uint32_t i = 0; i < h->path_count; i++)
        free(h->paths[i]);
    for (uint32_t i = 0; i < h->ignore_count; i++)
        free(h->ignore[i]);
    free_files(h);
    free(h->held);
    free(h->backends);
    free(h->phases);
    free(h->order);
    free(h->paths);
    free(h->ignore);
    config_free(&h->config);
}
