# Copyright (C) 2026 Enkrid
#
# Under the GNU Affero General Public License, version 3 only: see license.
import ctypes
import os
import signal
import subprocess
import sys
import threading

SIGNALS = {'nothing': 0, 'interrupt': 1, 'interrupt twice': 2}
ASKED = '? go on'
LINE = 'a signal stops the host: a second one ends it at once'
WAIT = 20


class Posix:
    def __init__(self, command):
        signal.signal(signal.SIGINT, signal.SIG_DFL)
        self.child = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                      stderr=subprocess.STDOUT)
        self.out, self.into = self.child.stdout, self.child.stdin

    def interrupt(self):
        self.child.send_signal(signal.SIGINT)

    def waited(self, seconds):
        try:
            return self.child.wait(seconds)
        except subprocess.TimeoutExpired:
            return None

    def stop(self):
        if self.child.poll() is None:
            self.child.kill()


class Windows:
    def __init__(self, command):
        import msvcrt
        from ctypes import wintypes as w
        self.k = ctypes.WinDLL('kernel32', use_last_error=True)
        self.w = w

        class Coord(ctypes.Structure):
            _fields_ = [('X', w.SHORT), ('Y', w.SHORT)]

        class Start(ctypes.Structure):
            _fields_ = [('cb', w.DWORD), ('reserved', w.LPWSTR), ('desktop', w.LPWSTR), ('title', w.LPWSTR)]
            _fields_ += [(name, w.DWORD) for name in ('x', 'y', 'width', 'height', 'columns', 'rows', 'fill', 'flags')]
            _fields_ += [('show', w.WORD), ('reserved2', w.WORD), ('reserved3', ctypes.c_void_p),
                         ('input', w.HANDLE), ('output', w.HANDLE), ('errors', w.HANDLE), ('attributes', ctypes.c_void_p)]

        class Process(ctypes.Structure):
            _fields_ = [('process', w.HANDLE), ('thread', w.HANDLE), ('pid', w.DWORD), ('tid', w.DWORD)]

        self.k.CreatePseudoConsole.argtypes = [Coord, w.HANDLE, w.HANDLE, w.DWORD, ctypes.POINTER(w.HANDLE)]
        self.k.ClosePseudoConsole.argtypes = [w.HANDLE]
        self.k.ClosePseudoConsole.restype = None
        self.k.UpdateProcThreadAttribute.argtypes = [ctypes.c_void_p, w.DWORD, ctypes.c_size_t, ctypes.c_void_p,
                                                     ctypes.c_size_t, ctypes.c_void_p, ctypes.c_void_p]
        self.k.CreateProcessW.argtypes = [w.LPCWSTR, w.LPWSTR, ctypes.c_void_p, ctypes.c_void_p, w.BOOL, w.DWORD,
                                          ctypes.c_void_p, w.LPCWSTR, ctypes.c_void_p, ctypes.c_void_p]
        tty_read, tty_write = self.pipe()
        screen_read, screen_write = self.pipe()
        self.console = w.HANDLE()
        self.check(self.k.CreatePseudoConsole(Coord(80, 25), tty_read, screen_write, 0,
                                              ctypes.byref(self.console)) == 0, 'CreatePseudoConsole')
        in_read, in_write = self.pipe()
        out_read, out_write = self.pipe()
        for handle in (in_read, out_write):
            self.k.SetHandleInformation(handle, 1, 1)
        size = ctypes.c_size_t()
        self.k.InitializeProcThreadAttributeList(None, 1, 0, ctypes.byref(size))
        self.attributes = ctypes.create_string_buffer(size.value)
        self.check(self.k.InitializeProcThreadAttributeList(self.attributes, 1, 0, ctypes.byref(size)), 'attributes')
        self.check(self.k.UpdateProcThreadAttribute(self.attributes, 0, 0x00020016, self.console,
                                                    ctypes.sizeof(w.HANDLE), None, None), 'pseudoconsole')
        start = Start(cb=ctypes.sizeof(Start), flags=0x100, input=in_read, output=out_write, errors=out_write,
                      attributes=ctypes.cast(self.attributes, ctypes.c_void_p))
        self.info = Process()
        self.k.SetConsoleCtrlHandler(None, False)
        line = ctypes.create_unicode_buffer(subprocess.list2cmdline(command))
        self.check(self.k.CreateProcessW(None, line, None, None, True, 0x80000, None, None,
                                         ctypes.byref(start), ctypes.byref(self.info)), 'CreateProcessW')
        for handle in (tty_read, screen_write, in_read, out_write):
            self.k.CloseHandle(handle)
        threading.Thread(target=drain, args=(os.fdopen(msvcrt.open_osfhandle(screen_read.value, 0), 'rb'),),
                         daemon=True).start()
        self.tty = os.fdopen(msvcrt.open_osfhandle(tty_write.value, 0), 'wb', buffering=0)
        self.out = os.fdopen(msvcrt.open_osfhandle(out_read.value, 0), 'rb')
        self.into = os.fdopen(msvcrt.open_osfhandle(in_write.value, 0), 'wb')

    def pipe(self):
        read, write = self.w.HANDLE(), self.w.HANDLE()
        self.check(self.k.CreatePipe(ctypes.byref(read), ctypes.byref(write), None, 0), 'CreatePipe')
        return read, write

    def check(self, done, what):
        if not done:
            sys.exit('%s failed: error %d' % (what, ctypes.get_last_error()))

    def interrupt(self):
        self.tty.write(b'\x03')

    def waited(self, seconds):
        code = self.w.DWORD()
        if self.k.WaitForSingleObject(self.info.process, seconds * 1000) != 0:
            return None
        self.k.GetExitCodeProcess(self.info.process, ctypes.byref(code))
        return code.value

    def stop(self):
        if self.waited(0) is None:
            self.k.TerminateProcess(self.info.process, 3)
        threading.Thread(target=self.k.ClosePseudoConsole, args=(self.console,), daemon=True).start()


def drain(out):
    while out.read(1 << 16):
        pass


def relay(out, seen):
    for line in iter(out.readline, b''):
        said = line.decode('utf-8', 'replace').rstrip('\r\n')
        print(said, flush=True)
        if said.strip() in seen:
            seen[said.strip()].set()


def main():
    program, config, count = sys.argv[1], sys.argv[2], SIGNALS[sys.argv[3]]
    child = (Windows if os.name == 'nt' else Posix)([program, config])
    seen = {ASKED: threading.Event(), LINE: threading.Event()}
    reader = threading.Thread(target=relay, args=(child.out, seen))
    reader.start()
    seen[ASKED].wait(WAIT)
    for _ in range(count):
        child.interrupt()
        seen[LINE].wait(WAIT)
    status = child.waited(WAIT) if count > 1 else None
    if status is None:
        try:
            child.into.write(b'go\n')
            child.into.close()
        except OSError:
            pass
        status = child.waited(WAIT)
    child.stop()
    reader.join(WAIT)
    if status is None:
        print('the host did not end in %d seconds' % WAIT, flush=True)
    sys.exit(3 if status is None else status)


if __name__ == '__main__':
    main()
