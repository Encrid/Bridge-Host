# Bridge-Host

Bridge-Host is the reference host of Bridge, and the code that a host of Bridge shares.

## Background

The project has these parts. Each part has its own releases: `docs/contributing/pull_request.md`, *The release*.

| Part | Folder | Content |
| --- | --- | --- |
| `host` | `host` | The program `bridge-host`, which runs a deployment of modules: `docs/extern/host/readme.md`. |

## Install

1. Install Python 3.11 or a later version.
2. Run `python project.py fetch`. It downloads zig, Bridge, mosl, BB-Native, BB-Wasm and binaryen.

## Usage

| Command | Result |
| --- | --- |
| `python project.py fetch` | Downloads what the build needs. |
| `python project.py build` | Builds the parts. |
| `python project.py pack` | Packs each part for a release. |
| `python project.py test` | Builds the parts, and the programs and fixtures of the tests. Then runs the tests. |
| `python project.py test --break` | Runs the tests with their claims made false. Each test must fail. |
| `python project.py test --branches` | Runs the tests on a build with probes. Each branch of the code must run. |
| `python project.py test --leaks` | Runs the tests on a build with counts. Each test must give back what the code takes. |
| `python project.py check` | Checks the project. |
| `python project.py clean` | Removes what the build made. Keeps what `fetch` downloaded. |
| `python project.py distclean` | Removes the folder `build`, also what `fetch` downloaded. |

Each command takes `--part <part>`, and then acts on that part and on the parts that it uses.

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
