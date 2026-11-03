"""Wysyla plik z PC na telefon przez USB: telnet uruchamia `nc -l`, a PC wysyla bajty. Sprawdza rozmiar i sha256.
python phone_push.py LOKALNY ZDALNY [--port 9000] [--mode 755]
Zdalny katalog musi istniec. Wazne: PC laczy sie do telefonu (ruch wychodzacy, zapora Windows nie przeszkadza)."""
import socket, sys, time, hashlib, argparse, os, shlex, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from phone_shell import run, HOST
ap = argparse.ArgumentParser(); ap.add_argument("local"); ap.add_argument("remote")
ap.add_argument("--port", type=int, default=9000); ap.add_argument("--mode", default="")
ap.add_argument("--timeout", type=int, default=300, help="send/receiver-completion timeout, seconds")
a = ap.parse_args()
if not 1 <= a.port <= 65535 or not 1 <= a.timeout <= 3600:
    ap.error("invalid port/timeout")
if a.mode and not re.fullmatch(r"[0-7]{3,4}",a.mode):
    ap.error("mode must be octal")
target = shlex.quote(a.remote)
data = open(a.local, "rb").read()
_, rc, ok = run("(nc -l -p %d > %s &)\nsleep 1" % (a.port, target), 15)
if not ok or str(rc) != '0':
    raise SystemExit("Cannot start phone receiver")
s = socket.create_connection((HOST, a.port), timeout=15)
s.settimeout(a.timeout); s.sendall(data); s.shutdown(socket.SHUT_WR); s.close()
cmd = f'''(
tries=0
while [ "$tries" -lt {a.timeout} ]; do
  size=$(wc -c < {target})
  [ "$size" -lt {len(data)} ] || break
  tries=$((tries+1)); sleep 1
done
printf 'MFW_PUSH_SIZE=%s\\n' "$size"
[ "$size" -eq {len(data)} ] || exit 1
printf 'MFW_PUSH_SHA=%s\\n' "$(sha256sum {target} | cut -d' ' -f1)"
''' + (f"chmod {a.mode} {target}\n" if a.mode else "") + ")"
out, rc, ok = run(cmd, a.timeout+15)
size_match = re.search(r'^MFW_PUSH_SIZE=(\d+)\s*$',out,re.M)
sha_match = re.search(r'^MFW_PUSH_SHA=([0-9a-f]{64})\s*$',out,re.M)
if not ok or str(rc) != '0' or not size_match or not sha_match:
    raise SystemExit("Phone receiver completion/checksum not confirmed; do not use the file")
size, sha = size_match.group(1), sha_match.group(1)
good = int(size) == len(data) and sha == hashlib.sha256(data).hexdigest()
print("%s -> %s: %s bajtow, %s" % (a.local, a.remote, size, "OK" if good else "BLAD sha"))
sys.exit(0 if good else 1)
