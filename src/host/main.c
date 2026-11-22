/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"
#include "os.h"

constexpr char STOP[] = "a signal stops the host: "
    "a second one ends it at once\n";

static int say_usage(void)
{
    say("usage: bridge-host <config.toml>\n"
        "       bridge-host --show-config <config.toml>\n");
    return (2);
}

int main(int argc, char **argv)
{
    static host_t h;
    bool show = argc == 3 && !strcmp(argv[1], "--show-config");
    char *named = argc == 2 || show ? argv[argc - 1] : nullptr;
    int status = 1;

    if (!named || named[0] == '-')
        return (say_usage());
    if (os_signal_catch(STOP)) {
        say("cannot catch the signals\n");
        return (1);
    }
    status = host_go(&h, named, show);
    host_free(&h);
    return (status);
}
