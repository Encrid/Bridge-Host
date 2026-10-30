# Bridge-Host

Bridge-Host is the reference host of Bridge: a program that runs a deployment of modules.

## Background

`bridge-host` runs the modules of a deployment file. It reads the file and its `.d/local.toml` overlay.
Then it reads the manifest of each module, loads the modules that a backend holds, and runs the phases in rounds.
It links the library of Bridge, and opens each backend that the deployment names, for example `bridge_native` of BB-Native or `bridge_wasm` of BB-Wasm.

## Install

1. Install Python 3.11 or a later version.
2. Run `python project.py fetch`. It downloads zig, Bridge, Bridge-OS, BB-Native, BB-Wasm and binaryen.

## Usage

| Command | Result |
| --- | --- |
| `python project.py fetch` | Downloads what the build needs. |
| `python project.py build` | Builds the host. |
| `python project.py pack` | Packs the host for a release. |
| `python project.py test` | Builds the host, and the programs and fixtures of the tests. Then runs the tests. |
| `python project.py test --break` | Runs the tests with their claims made false. Each test must fail. |
| `python project.py test --branches` | Runs the tests on a build with probes. Each branch of the code must run. |
| `python project.py test --leaks` | Runs the tests on a build with counts. Each test must give back what the code takes. |
| `python project.py check` | Checks the project. |
| `python project.py clean` | Removes what the build made. Keeps what `fetch` downloaded. |
| `python project.py distclean` | Removes the folder `build`, also what `fetch` downloaded. |

After `python project.py build`, run `bridge-host <config.toml>`, or `bridge-host --show-config <config.toml>` to print the deployment without running it.
`--show-config` prints each key and its whole value, with the file and the line that state it.
The host gives each line to the output in one piece, when the line is complete.
The host exits with 0 after a run, and with 2 after it prints its usage.
It exits with 1 if it refuses a key, if Bridge refuses a call, or if a record is an error.

## The deployment file

A deployment is a TOML file. The document of the header `bridge.h` of Bridge tells what each value means to Bridge.

### The files

| File | Role |
| --- | --- |
| The file that the command names | The deployment. |
| A file of `presets` | A file that gives keys to the file that names it. A preset can name presets too. |
| `<file>.d/local.toml` | The local file. It is over each other file. |
| `<file>.d/lock` | The host holds it during a run, so only one run of a deployment goes on at a time. |

- A path counts from the folder of the file that states it: a preset, a path of `[content]`, a library of `[backend]`, and `{ path = "..." }`.
- A file is over the files that it names. If two presets of one file give different values for a key, the host refuses the key.
- The paths of `[content]` and the pairs of `[order]` of all the files add up.
- For `read` and `write`, the first file that states the list gives the whole list. If a file states no list, the lists of the presets that it names add up.
- The host refuses a file that names itself, and reads a file that two files name only one time.
- It refuses a key that a file states twice, a key that it does not know, and an empty table.
- The local file cannot state `presets`, the keys of the world, `[budget]`, `[backend]` or `[checks]`.

### The world

| Key | Value |
| --- | --- |
| `presets` | A list of files. |
| `phases` | The names of the phases. A round runs them in this order. |
| `threads` | How many threads run a phase, from 1. |
| `rounds` | The most rounds of the run, from 0 to 1000000. 0 is no limit. |
| `live` | `false`: a quiet round ends the run. `true`: a quiet round does not end it. |

`[budget]` holds the 10 limits of the world, and each one is needed.
`entities`, `tables`, `types`, `systems`, `terms`, `modules`, `persistent` and `columns` take a whole number from 1.
`sparse_sets` and `direct_indexed` take a whole number from 1, or `"none"`.

The table `[checks]` holds two switches, `profile` and `validate`, each `true` or `false`.
A check that no file states is off, and the host writes no line for it.
If two files state different values for a check, the host refuses it, as it refuses each other key.
The document of the header `bridge.h` of Bridge tells what each check does.

`[order] before` is a list of pairs of systems, each with its full name `<namespace>:<name>:<system>`.
In one phase, the first system of a pair runs before the second.
Two systems of two modules that conflict in one phase need a pair, or Bridge refuses the second module.

### The content and the backends

| Key | Value |
| --- | --- |
| `[backend] <name>` | The library of a backend, with no suffix. The host adds `.dll`, `.so` or `.dylib`. |
| `[content] paths` | The files and the folders of the modules. Each one is needed. |
| `[content] ignore` | The paths that the host does not read, with each path below them. |
| `[content.load] <identity>` or `all` | `true` loads the module, `false` does not. The key of an identity is over `all`. |
| `[content.arrive] <backend>` | `"ask"`, `"load"` or `"leave"`: what the host does with a module that comes after the first look. |

A module asks for the backend `native` or `wasm` by the format of its file.
Each backend needs its key of `[content.arrive]`.
With `"ask"`, the host writes `? trust <identity>, <backend>, <file>`, and reads one whole line.
It loads the module only if the line is `yes`.

The host reads a folder one level deep.
A path of the content has no limit of length, but a system can refuse to open a long path.

It looks at 32 files at most, and it does not read a file for which it has no room.
Each time its room becomes full, it writes `too many files: <count> are looked at`.
At each round, it loads a new module and unloads the module of a removed file.
It replaces the module of a changed file only if the manifest states `reloadable=true` and the deployment states `hot_reload = true`.
Else it keeps the old module, and writes why.

### The modules

Three kinds of table hold the keys of a module:

- `[module."<namespace>:<name>@<version>"]` holds them for one module.
- `[module.<backend>]` holds them for each module of a backend.
- `[module.all]` holds them for each module.

In one file, the table of the identity is over the table of the backend, and that one is over `all`.
If no file states a key, the host takes the default that the manifest gives.
If the manifest gives none, the host writes `not stated: <key>`, and does not load the module.

| Key | Value |
| --- | --- |
| `hot_reload` | `true` or `false`. A reload also needs `reloadable=true` in the manifest. |
| `on_error` | `"fail"` or `"skip"`. |
| `max_system_failures` | A whole number from 1, or `"none"`, which is no limit. |
| `work_limit`, `memory_limit`, `init_timeout`, `reload_timeout`, `shutdown_timeout` | A whole number from 1, or `"none"`, which is no limit. A limit asks a capability of the backend. The backend of BB-Native has none of them, so it needs `"none"`. |
| `log_level` | A level, from 1 to 7. |
| `scratch`, `log_ring`, `spans` | A whole number from 1. |
| `spawns`, `persistent`, `changes` | A whole number from 1, or `"none"`, which is none at all. |
| `losses` | A list of names, or `"none"`. |
| `read`, `write` | The lists of the component names that the module may read and write. |

Each key fills the field of the same name of `bridge_module_config_t` of Bridge.
Only the manifest states `abi`, `config`, `settings`, `pinned` and `reloadable`. The host refuses them in a deployment.

`[module.<table>.phases]` maps a phase of the module to a phase of the world, as `<name> = "<phase>"`. A phase with no map keeps its name.
`[module."<identity>".config]` gives the settings of one module, as `<setting> = <value>`.
A value is a text, a whole or real number, `true` or `false`, or `{ path = "<path>" }`, a path from the folder of the file.
Only the settings that the manifest names are allowed, and each one needs a value, from a file or from the manifest.

## The end of a run

A round is quiet if the look finds no change in the content and no phase writes a record.
The run also ends at the limit of `rounds`, and when Bridge refuses a call.
A system or an observer that gives `BRIDGE_END` ends the run after its round, with each value of `live`.
The document of the header `bridge_abi.h` of Bridge tells the status `BRIDGE_END`.
The host writes each record of a level from 0 to 6.

## The manifest of a module

A module states its manifest in its file, as words that each end with a zero byte.

| Format | Place |
| --- | --- |
| PE, on Windows, and ELF, on Linux | The section `.bridge`. |
| Mach-O, on macOS | The section `__TEXT,.bridge`. |
| wasm | The custom section `bridge:manifest`. |

| Word | Value |
| --- | --- |
| The first word | The identity `<namespace>:<name>@<version>`, of at most 127 bytes, as Bridge takes it. |
| `abi=<number>` | Needed. The ABI of Bridge that the module was built against. |
| `config=<component>` or `config=none` | Needed. The component that holds the settings. |
| `pinned=true` or `pinned=false` | Needed. `true`: the systems of the module run on the thread that runs the phases. |
| `reloadable=true` or `reloadable=false` | Needed. `true`: the host can replace the module while the run goes on. |
| `settings=<names>` | The names of the settings, with a space between two names. |
| `<key>=<value>` | The default of a key of the module, for example `scratch=65536` or `read=["game:Body"]`. |
| `config.<name>=<value>` | The default of a setting. |

Only the identity has a limit of length. No other word of the manifest has one.
The host refuses a word with no `=` after the first word. It does not read a word with a key that it does not know.

It refuses a word that it reads if the manifest states its key twice.
For `abi`, `config`, `settings`, `pinned` and `reloadable`, it writes `states <key> twice`. For a default, it writes `its file states <key> twice`.
In the message `states a <word> that cannot be`, `<word>` is the part before the first `=`, cut to its first 60 bytes.

## Messages

With no file, or a bad one, the host prints its usage:

```text
usage: bridge-host <config.toml>
       bridge-host --show-config <config.toml>
```

Reading the deployment:

- `cannot go into '<file>'`
- `cannot read '<file>'`
- `not stated: backend`
- `not stated: content.paths`
- `not stated: <key>`
- `refused: <file>:<line>: <why>`
- `refused: <file>:<line>: <key>: <why>`
- `refused: <file>:<line>: <identity>: no module found is it`
- `refused: <file>:<line>: presets: a list of files`
- `refused: <file> names itself:`
- `refused: <key>: <value> in <file>:<line>, <value> in <file>:<line>`
- `order <system> <system>`
- `, over <file>:<line>`
- `, named by <file>:<line>`
- `<file>.d/local.toml`

Holding one instance:

- `refused: <file>.d is no folder`
- `refused: <file> is being run`
- `<file>.d/lock`
- `cannot lock '<lock>'`

Finding the modules:

- `cannot look in '<path>'`
- `too many files: <count> are looked at`
- `not a module: <file>: <why>`
- `neither a wasm module nor a library`
- `no manifest`
- `states a <word> that cannot be`
- `states <key> twice`
- `its manifest does not begin with who it is`
- `its identity has more than 127 bytes`
- `its manifest has a word with no zero byte at its end`
- `its manifest is empty`
- `states no abi`
- `unloaded: <identity>: <file> is gone`

Loading a module:

- `loaded: <identity>, <backend>, <file>`
- `? trust <identity>, <backend>, <file>`
- `not loaded: <file>: <identity> is <file>`
- `not loaded: <file>: <identity> <why>`
- `not loaded: <file>: <identity>, <backend>, not trusted`
- `not loaded: <file>: <identity> was built against another ABI`
- `not loaded: <file>: <identity> asks for <backend>, which the host was not given`
- `not loaded: <file>: <identity>, as content.load says`
- `not loaded: <file>: <identity> was refused`
- `not loaded: <file>: <identity>:`

Replacing a module:

- `replaced: <identity> by <identity>, <file>`
- `not replaced: <identity>: it does not say it can be`
- `not replaced: <identity>: its hot_reload is false`
- `not replaced: <file>: <identity> was refused`
- `not replaced: <file>: <identity> <why>`
- `not replaced: <file>: <identity>:`

Running the rounds:

- `bridge_create refused`
- `record[<level>] <identity>/<system>: <text> <number> <number>`
- `ended after <rounds> rounds, as many as it was given`
- `ended at round <round>: a module asked for the end`
- `still loaded:`
- `destroyed: <count>`

Showing the deployment:

- `file <file>`
- `, the local file`
- `found:`
- `module <identity>, <backend>, <file>`
- `content.paths: <path>`
- `content.ignore: <path>`
- `order: <system> before <system>`
- `<key> = [<names>]`
- `config = "<component>"  its file`
- `settings = "<names>"  its file`
- `pinned = <flag>  its file`
- `reloadable = <flag>  its file`
- `its file states <key>: <why>`
- `its file states <key> twice`
- `its file states no config`
- `its file states no pinned`
- `its file states no reloadable`
- `it takes no config.<name>, stated in <file>:<line>`
- `it names no component, and settings`
- `phases.<name>`

## Contributing

| Document | Content |
| --- | --- |
| `docs/contributing/needs.md` | The needs that each decision meets. |
| `docs/contributing/decisions.md` | The steps of a decision. |
| `docs/contributing/coding_style.md` | How to write the code. |
| `docs/contributing/documentation_style.md` | How to write the documents. |
| `docs/contributing/tests.md` | How to write the tests. |
| `docs/contributing/commits.md` | How to write a commit. |
| `docs/contributing/pull_request.md` | How to write a pull request. |
| `docs/contributing/agreement.md` | The agreement `cla.md`, which each contributor signs. |

## License

The GNU Affero General Public License, version 3 only. The file `license` holds the text.

The host links the library of Bridge, thus it is under the same licence, and its source goes with it.
