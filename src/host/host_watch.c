/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static int take_names(host_t *h, seeing_t *s)
{
    const char *name = nullptr;
    char *full = nullptr;
    int whole = 0;
    int got = os_watch_take(h->watches[s->at], &name);

    for (; got == 1; got = os_watch_take(h->watches[s->at], &name)) {
        full = join_path(h->paths[s->at], name);
        if (!full || (!is_ignored(h, full) && !walk_name(full, see_file, s)))
            whole = 1;
        free(full);
    }
    return (got ? 2 : whole);
}

int look_path(host_t *h, enum look_kind kind, seeing_t *s)
{
    os_watch_t **w = &h->watches[s->at];
    int taken = kind == LOOK_NOTICES && *w ? take_names(h, s) : 1;
    int read = 0;

    if (kind == LOOK_FIRST || taken == 2) {
        os_watch_stop(*w);
        *w = os_watch_start(h->paths[s->at], h->d.depth);
    }
    if (taken)
        read = read_path(h, s);
    if (read == 1 && !*w && (kind == LOOK_FIRST || taken == 2))
        say("cannot watch '%s': read at each round\n", h->paths[s->at]);
    return (read);
}

void retry_unread(host_t *h, seeing_t *s)
{
    for (uint32_t i = 0; i < h->d.files; i++) {
        s->at = h->files[i].at;
        if (h->files[i].unread)
            (void)walk_name(h->files[i].path, see_file, s);
    }
}

void stop_watches(host_t *h)
{
    for (uint32_t i = 0; h->watches && i < h->path_count; i++)
        os_watch_stop(h->watches[i]);
    free(h->watches);
}
