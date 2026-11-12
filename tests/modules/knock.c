/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

#include "bridge_native.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

static bool said;

#ifdef END_AT
static uint32_t rounds;
#endif

#ifdef AGAIN_TIMES
static uint32_t runs;
#endif

#if defined(WAIT) || defined(WAIT_EACH)

static void wait_for_notices(void)
{
#ifdef _WIN32
    Sleep(250);
#else
    nanosleep(&(struct timespec){ .tv_nsec = 250000000 }, nullptr);
#endif
}

static void go_on(void)
{
    char answer[64];

    printf("? go on\n");
    fflush(stdout);
    if (!fgets(answer, sizeof(answer), stdin))
        answer[0] = 0;
    wait_for_notices();
}

#endif

BRIDGE_NATIVE(say)
{
    bridge_line_t line = { "knock", 5, 0, 0 };

#ifdef AGAIN_TIMES
    runs++;
    line = (bridge_line_t){ "run", 3, runs, 0 };
    bridge_native_log(api, BRIDGE_LOG_INFO, &line);
    return (runs <= AGAIN_TIMES ? BRIDGE_AGAIN : 0);
#endif
#ifdef END_AT
    rounds++;
    if (rounds == END_AT)
        return (BRIDGE_END);
#endif
#ifdef WAIT
    if (!said)
        go_on();
#endif
#ifdef WAIT_EACH
    go_on();
#endif
    if (!said)
        bridge_native_log(api, BRIDGE_LOG_INFO, &line);
    said = true;
    return (0);
}

BRIDGE_NATIVE(init)
{
    int none = 0;

    BRIDGE_NATIVE_ADD_SYSTEM(api, "say", "update", &none, 0, nullptr, 0,
        nullptr, 0);
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
#ifdef STOP
    printf("shutdown %s\n", STOP);
    fflush(stdout);
#endif
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
