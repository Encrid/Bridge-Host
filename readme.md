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

The deployment file states these keys at its top:

| Key | Value |
| --- | --- |
| `phases` | The names of the phases. A round runs them in this order. |
| `threads` | How many threads run a phase, from 1. |
| `rounds` | The most rounds of the run, from 0 to 1000000. 0 is no limit. |
| `live` | `false`: a quiet round ends the run. `true`: a quiet round does not end it. |

A round is quiet if the look finds no change in the content and no phase writes a record.
The run also ends at the limit of `rounds`, and when Bridge refuses a call.
A system or an observer that gives `BRIDGE_END` ends the run after its round, with each value of `live`.
The document of the header `bridge_abi.h` of Bridge tells the status `BRIDGE_END`.
After a run, the host exits with 0, or with 1 if Bridge refused a call or a record is an error.
`--show-config` prints each of these keys, with the file and the line that state it.

## Messages

With no file, or a bad one, the host prints its usage:

```text
usage: bridge-host <config.toml>
       bridge-host --show-config <config.toml>
```

Reading the deployment:

- `cannot go into '%s'`
- `cannot read '%s'`
- `not stated: backend`
- `not stated: content.paths`
- `not stated: %s`
- `refused: %s:%u: %s`
- `refused: %s:%u: %s: %s`
- `refused: %s:%u: %s: no module found is it`
- `refused: %s:%u: presets: a list of files`
- `refused: %s names itself:`
- `refused: %s: %s in %s:%u, %s in %s:%u`
- `order %s %s`
- `, over %s:%u`
- `, named by %s:%u`
- `%s.d/local.toml`

Holding one instance:

- `refused: %s.d is no folder`
- `refused: %s is being run`
- `%s.d/lock`
- `cannot lock '%s'`

Finding the modules:

- `cannot look in '%s'`
- `too many files: %d are looked at`
- `not a module: %s: %s`
- `states a %.60s that cannot be`
- `unloaded: %s: %s is gone`

Loading a module:

- `loaded: %s, %s, %s`
- `? trust %s, %s, %s`
- `not loaded: %s: %s is %s`
- `not loaded: %s: %s %s`
- `not loaded: %s: %s, %s, not trusted`
- `not loaded: %s: %s was built against another ABI`
- `not loaded: %s: %s asks for %s, which the host was not given`
- `not loaded: %s: %s, as content.load says`
- `not loaded: %s: %s was refused`
- `not loaded: %s: %s:`

Replacing a module:

- `replaced: %s by %s, %s`
- `not replaced: %s: it does not say it can be`
- `not replaced: %s: %s was refused`
- `not replaced: %s: %s%s%s`
- `not replaced: %s: %s:`

Running the rounds:

- `bridge_create refused`
- `record[%d] %s/%s: %.*s %u %u`
- `ended after %d rounds, as many as it was given`
- `ended at round %d: a module asked for the end`
- `still loaded:`
- `destroyed: %d`

Showing the deployment:

- `found:`
- `module %s, %s, %s`
- `content.paths: %s`
- `content.ignore: %s`
- `order: %s before %s`
- `config = "%s"  its file`
- `settings = "%s"  its file`
- `pinned = %s  its file`
- `reloadable = %s  its file`
- `its file states %s: %s`
- `its file states no config`
- `its file states no pinned`
- `its file states no reloadable`
- `it takes no config.%s, stated in %s:%u`
- `it names no component, and settings`
- `phases.%s`

## Contributing

| Document | Content |
| --- | --- |
| `docs/contributing/coding_style.md` | How to write the code. |
| `docs/contributing/documentation_style.md` | How to write the documents. |
| `docs/contributing/tests.md` | How to write the tests. |
| `docs/contributing/commits.md` | How to write a commit. |
| `docs/contributing/pull_request.md` | How to write a pull request. |
| `docs/contributing/agreement.md` | The agreement `cla.md`, which each contributor signs. |

## License

The GNU Affero General Public License, version 3 only. The file `license` holds the text.

The host links the library of Bridge, thus it is under the same licence, and its source goes with it.
For a work that stays closed, other terms are for sale: ask the account `Encrid` on the service that hosts this repository.
