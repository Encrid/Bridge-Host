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

static bool take_config(host_t *h, const char *name)
{
    bool taken = config_read(&h->config, name);

    taken = taken && !config_check(&h->config);
    if (!taken)
        return (false);
    taken = config_world(h);
    taken = config_backends(h) && taken;
    return (config_content(h) && taken);
}

static int go(host_t *h, char *named, bool show)
{
    const char *name = config_go_beside(named);
    bool taken = name && take_config(h, name);

    if (!name)
        say("cannot go into '%s'\n", named);
    if (show)
        return (show_config(h, taken));
    if (taken && take_lock(name))
        return (run(h));
    return (1);
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
    status = go(&h, named, show);
    host_free(&h);
    return (status);
}
