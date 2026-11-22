/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "say.h"
#include "say_told.h"

constexpr size_t TEXT_MOST = 1024;

bool say_told;

static void put(const char *format, va_list args)
{
    char text[TEXT_MOST] = {};
    va_list again;

    va_copy(again, args);
    vsnprintf(text, sizeof(text), format, args);
    say_told = say_told || strstr(text, "no memory");
    vprintf(format, again);
    va_end(again);
}

void say_part(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    put(format, args);
    va_end(args);
}

void say(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    put(format, args);
    va_end(args);
    fflush(stdout);
}
