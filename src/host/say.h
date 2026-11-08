/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BRIDGE_HOST_SAY_H
    #define BRIDGE_HOST_SAY_H

    #include <stddef.h>

typedef struct {
    char *text;
    size_t used;
} say_line_t;

[[gnu::format(printf, 1, 2)]] void say_part(const char *format, ...);

[[gnu::format(printf, 1, 2)]] void say(const char *format, ...);

#endif
