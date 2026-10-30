# Copyright (C) 2026 Enkrid
#
# Under the GNU Affero General Public License, version 3 only: see license.
import os
import shutil
import sys
from pathlib import Path

NAME = 'd' * 200


def unreadable(path):
    if os.name == 'nt':
        import _winapi
        handle = _winapi.CreateFile(str(Path(path).resolve()), _winapi.GENERIC_READ, 0, 0,
                                    _winapi.OPEN_EXISTING, 0, 0)
        return lambda: _winapi.CloseHandle(handle)
    mode = os.stat(path).st_mode
    os.chmod(path, 0)
    return lambda: os.chmod(path, mode)


def deep_windows(folder, levels, source):
    top = '\\\\?\\' + os.path.abspath(folder)
    inner = os.path.join(top, *[NAME] * levels)
    os.makedirs(inner)
    if source != 'none':
        shutil.copyfile(source, os.path.join(inner, os.path.basename(source)))
    return lambda: shutil.rmtree(top)


def deep_posix(folder, levels, source):
    os.makedirs(folder)
    at = os.open(folder, os.O_RDONLY)
    for _ in range(levels):
        os.mkdir(NAME, dir_fd=at)
        below = os.open(NAME, os.O_RDONLY, dir_fd=at)
        os.close(at)
        at = below
    if source != 'none':
        out = os.open(os.path.basename(source), os.O_WRONLY | os.O_CREAT, 0o644, dir_fd=at)
        os.write(out, Path(source).read_bytes())
        os.close(out)
    os.close(at)
    return lambda: shutil.rmtree(folder)


def main():
    if sys.argv[1] == 'unreadable':
        undo = unreadable(sys.argv[2])
    else:
        made = deep_windows if os.name == 'nt' else deep_posix
        undo = made(sys.argv[2], int(sys.argv[3]), sys.argv[4])
    print('ready', flush=True)
    sys.stdin.read()
    undo()


main()
