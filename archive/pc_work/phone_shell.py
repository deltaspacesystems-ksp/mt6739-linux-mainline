"""Klient powloki telefonu po lacu USB (telnetd na 192.168.7.2:23, bez hasla - TYLKO TEST).
Python 3.13 nie ma telnetlib, wiec surowe gniazdo + odpowiedzi na negocjacje IAC.
Uzycie:  python phone_shell.py "cat /proc/partitions"
         python phone_shell.py --file skrypt.sh        (tresc pliku wykonana w jednej sesji)
Wypisuje wyjscie polecenia, na koncu 'rc=<kod>'. Timeout: --timeout S (domyslnie 40)."""
import socket, sys, time, argparse, re

HOST, PORT = "192.168.7.2", 23
IAC, DONT, DO, WONT, WILL, SB, SE = 255, 254, 253, 252, 251, 250, 240
MARK = "__Y5END__"

def completed_status(data):
    """Only a complete result line ends a command, never its terminal echo.

    Require the newline too: a split status127 must not be accepted as1.
    """
    pattern = rb"(?m)^" + re.escape(MARK.encode()) + rb"[ \t]+([0-9]{1,3})[ \t]*\n"
    match = re.search(pattern, data.replace(b"\r", b""))
    if match and int(match.group(1)) <= 255:
        return match.group(1).decode("ascii")
    return None

def run(script, timeout):
    s = socket.create_connection((HOST, PORT), timeout=10)
    s.settimeout(1.0)
    buf = b""
    def pump(t):
        nonlocal buf
        end = time.time() + t
        while time.time() < end:
            try:
                d = s.recv(4096)
            except socket.timeout:
                continue
            if not d:
                return False
            i = 0
            while i < len(d):
                b = d[i]
                if b == IAC and i + 1 < len(d):
                    c = d[i + 1]
                    if c in (DO, DONT, WILL, WONT) and i + 2 < len(d):
                        s.sendall(bytes([IAC, WONT if c == DO else DONT, d[i + 2]]))
                        i += 3
                        continue
                    i += 2
                    continue
                buf += bytes([b])
                i += 1
            if completed_status(buf) is not None:
                return True
        return False
    pump(1.5)
    buf = b""
    s.sendall(("exec 2>&1\n" + script.replace("\r", "") + "\necho " + MARK + " $?\n").encode())
    ok = pump(timeout)
    s.close()
    text = buf.decode("utf-8", "replace").replace("\r", "")
    # po echo komend telnetd: wytnij wszystko do ostatniego znacznika i kod
    out, rc = text, "?"
    if MARK in text:
        head, _, tail = text.rpartition(MARK)
        rc = tail.strip().split()[0] if tail.strip() else "?"
        # wywal linie z echem polecenia (zawieraja 'echo __Y5END__')
        lines = [l for l in head.split("\n") if ("echo " + MARK) not in l]
        out = "\n".join(lines)
    return out, rc, ok

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("cmd", nargs="?")
    ap.add_argument("--file")
    ap.add_argument("--timeout", type=int, default=40)
    a = ap.parse_args()
    script = open(a.file, encoding="utf-8").read() if a.file else (a.cmd or "uname -a")
    out, rc, ok = run(script, a.timeout)
    sys.stdout.write(out.rstrip() + "\n")
    print("rc=%s%s" % (rc, "" if ok else " (timeout)"))
