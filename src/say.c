/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "say.h"

static void emit(say_line_t *l)
{
    if (l->text)
        fwrite(l->text, 1, l->used, stdout);
    free(l->text);
    l->text = nullptr;
    l->used = 0;
}

static void add(const char *format, va_list args, bool last)
{
    static say_line_t line;
    va_list again;
    size_t size = 0;
    char *more = nullptr;

    va_copy(again, args);
    size = (size_t)vsnprintf(nullptr, 0, format, args) + 1;
    more = realloc(line.text, line.used + size);
    if (more) {
        line.text = more;
        line.used += (size_t)vsnprintf(more + line.used, size, format, again);
    }
    if (!more || last)
        emit(&line);
    if (!more)
        vprintf(format, again);
    va_end(again);
}

void say_part(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    add(format, args, false);
    va_end(args);
}

void say(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    add(format, args, true);
    va_end(args);
    fflush(stdout);
}
