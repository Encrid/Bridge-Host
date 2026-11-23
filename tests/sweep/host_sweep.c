/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "host.h"
#include "os.h"
#include "say_told.h"
#include "sweep.h"

constexpr char STOP[] = "a signal stops the host: "
    "a second one ends it at once\n";
constexpr size_t NAME_MOST = 4096;

static int turn(const char *path)
{
    static host_t h;
    char named[NAME_MOST] = {};
    int status = 0;

    memset(&h, 0, sizeof(h));
    memcpy(named, path, strlen(path));
    say_told = false;
    status = host_go(&h, named, false);
    host_free(&h);
    return (status);
}

int main(int argc, char **argv)
{
    unsigned at = 0;
    unsigned untold = 0;
    int status = 0;
    bool refused = false;

    const char *call = argc == 3 ? argv[2] : "realloc";

    if (argc < 2 || argc > 3 || strlen(argv[1]) >= NAME_MOST
        || os_signal_catch(STOP))
        return (2);
    do {
        sweep_refuse(call, ++at);
        status = turn(argv[1]);
        refused = sweep_is_refused();
        if (refused && !say_told)
            printf("turn %u: no line tells the refused call\n", at);
        untold += refused && !say_told;
    } while (refused);
    printf("%u refused calls, %u untold, then status %d\n",
        at - 1, untold, status);
    return (untold || status);
}
