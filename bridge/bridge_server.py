"""HTTP relay for the cloud Claude session: run shell commands on the phone, push/pull files.
Exposed to the internet through nginx/Cloudflare, so every request except /health must carry
"Authorization: Bearer <BRIDGE_TOKEN>". Set BRIDGE_TOKEN in the environment before starting;
the server refuses to start without it. BRIDGE_BIND defaults to 127.0.0.1 (use the LAN IP if
nginx runs on another machine, and then firewall the port to that machine only).
"""
import os, hmac, hashlib, sys, socket, threading, time, json
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))   # y5sh.py i pc_agent.py leza obok tego pliku
import y5sh
import pc_agent         # sterowanie PC: wylaczone, dopoki BRIDGE_PS=1 + BRIDGE_PS_TOKEN

LISTEN_PORT = 2324
BIND = os.environ.get("BRIDGE_BIND", "127.0.0.1")
# Tylko te adresy moga rozmawiac z mostem (adres PC z nginx). Pusty = bez filtra (nie zalecane).
ALLOW_FROM = {a.strip() for a in os.environ.get("BRIDGE_ALLOW_FROM", "").split(",") if a.strip()}
MAX_BODY = int(os.environ.get("BRIDGE_MAX_BODY", 64 * 1024 * 1024))
AUDIT_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "bridge_audit.log")


def audit(ip, op, detail):
    try:
        with open(AUDIT_PATH, "a", encoding="utf-8") as f:
            f.write(json.dumps({"t": time.strftime("%Y-%m-%d %H:%M:%S"), "ip": ip, "op": op, "detail": str(detail)[:300]},
                               ensure_ascii=False) + "\n")
    except OSError:
        pass


TOKEN = os.environ.get("BRIDGE_TOKEN", "")

# Podpisywanie zadan: oprocz tokena (Authorization: Bearer, sprawdzanego tez przez nginx) kazde zadanie niesie podpis
#   X-Y5-Sig = HMAC-SHA256(BRIDGE_SIGN_KEY, czas \n nonce \n METODA \n sciezka?query \n sha256(tresci))
# + naglowki X-Y5-Time, X-Y5-Nonce, X-Y5-Body-Sha256. Klucz podpisu NIE jest nigdzie wysylany i nie ma go w nginx,
# wiec sam wyciek tokena nie wystarczy, a przechwyconego zadania nie da sie powtorzyc ani zmienic.
SIGN_KEY = os.environ.get("BRIDGE_SIGN_KEY", "")
SIGN_WINDOW = 120                    # dopuszczalna rozbieznosc zegarow (s)
NONCES = {}                          # nonce -> wygasniecie
NONCE_LOCK = threading.Lock()


def check_signature(headers, method, raw_path, key):
    """-> (ok, zadeklarowany sha256 tresci)."""
    ts, nonce = headers.get("X-Y5-Time", ""), headers.get("X-Y5-Nonce", "")
    sig, bsha = headers.get("X-Y5-Sig", ""), headers.get("X-Y5-Body-Sha256", "").lower()
    try:
        if abs(time.time() - int(ts)) > SIGN_WINDOW:
            return False, ""
    except ValueError:
        return False, ""
    if not (16 <= len(nonce) <= 64) or len(bsha) != 64:
        return False, ""
    msg = "\n".join([ts, nonce, method, raw_path, bsha]).encode()
    good = hmac.new(key.encode(), msg, hashlib.sha256).hexdigest()
    if not hmac.compare_digest(good, sig.lower()):
        return False, ""
    now = time.time()
    with NONCE_LOCK:
        for n in [n for n, exp in NONCES.items() if exp < now]:
            del NONCES[n]
        if nonce in NONCES:
            return False, ""
        NONCES[nonce] = now + 2 * SIGN_WINDOW + 30
    return True, bsha


NEXT_PORT = [2900]
PORT_LOCK = threading.Lock()


def alloc_port():
    with PORT_LOCK:
        p = NEXT_PORT[0]
        NEXT_PORT[0] += 1
        if NEXT_PORT[0] > 2999:
            NEXT_PORT[0] = 2900
        return p


def push(local_bytes, remote, port):
    def listener():
        y5sh.run('nc -l -p %d > %s; echo recv_done' % (port, remote), timeout=30)
    t = threading.Thread(target=listener)
    t.start()
    time.sleep(3)
    s = socket.create_connection(('172.16.42.1', port), timeout=8)
    s.sendall(local_bytes)
    s.close()
    t.join(30)
    return y5sh.run('chmod 644 %s' % remote, timeout=10)


def pull(remote, port):
    out = {}
    def listener():
        srv = socket.socket()
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        srv.bind(("172.16.42.2", port))
        srv.listen(1)
        srv.settimeout(30)
        c, _ = srv.accept()
        buf = b""
        c.settimeout(10)
        try:
            while True:
                d = c.recv(1 << 20)
                if not d:
                    break
                buf += d
        except socket.timeout:
            pass
        out["data"] = buf
        c.close()
        srv.close()
    t = threading.Thread(target=listener)
    t.start()
    time.sleep(1.0)
    y5sh.run("nc 172.16.42.2 %d < %s" % (port, remote), timeout=30)
    t.join(35)
    return out.get("data", b"")


FAILS = {}            # ip -> [timestamps of failed auth attempts]
FAIL_LOCK = threading.Lock()
MAX_FAILS, WINDOW, LOCKOUT = 5, 600, 900


def locked_out(ip):
    now = time.time()
    with FAIL_LOCK:
        hits = [t for t in FAILS.get(ip, []) if now - t < LOCKOUT]
        FAILS[ip] = hits
        return len([t for t in hits if now - t < WINDOW]) >= MAX_FAILS


def record_fail(ip):
    with FAIL_LOCK:
        FAILS.setdefault(ip, []).append(time.time())


class Handler(BaseHTTPRequestHandler):
    def _client_ip(self):
        return self.headers.get("X-Real-IP") or self.client_address[0]

    def _peer_ok(self):
        """Polaczenie musi przyjsc od nginx (BRIDGE_ALLOW_FROM); inaczej X-Real-IP mozna by podrobic."""
        if ALLOW_FROM and self.client_address[0] not in ALLOW_FROM:
            print("[bridge] rejected peer", self.client_address[0], flush=True)
            self.close_connection = True
            return False
        return True

    def _read_body(self, n):
        """Czyta tresc zadania; przy wlaczonym podpisie sprawdza, czy zgadza sie z podpisanym sha256 (None = niezgodna)."""
        data = self.rfile.read(n)
        if getattr(self, "_bsha", None) is not None and hashlib.sha256(data).hexdigest() != self._bsha:
            return None
        return data

    def _authorized(self, token=None, sign_key=None):
        ip = self._client_ip()
        self._bsha = None
        if locked_out(ip):
            print("[bridge] locked out", ip, flush=True)
            return False
        got = self.headers.get("Authorization", "")
        ok = hmac.compare_digest(got.encode(), ("Bearer " + (token or TOKEN)).encode())
        key = SIGN_KEY if sign_key is None else sign_key
        if ok and key:
            ok, self._bsha = check_signature(self.headers, self.command, self.path, key)
        if not ok:
            record_fail(ip)
            print("[bridge] auth failed from", ip, flush=True)
        return ok

    def _send(self, code, body, content_type="application/octet-stream"):
        self.send_response(code)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _pc(self, method, parsed):
        """Sciezki /pc/*: tylko gdy wlaczone i tylko z osobnym tokenem (token do telefonu tu nie dziala)."""
        if not pc_agent.active():           # wylaczone, wygaslo (TTL) albo jest plik DISABLE_PC
            self._send(404, b'{"ok": false, "error": "not found"}', "application/json")
            return
        if not self._authorized(pc_agent.PS_TOKEN, pc_agent.PS_SIGN_KEY):
            self._send(401, b'{"ok": false, "error": "unauthorized"}', "application/json")
            return
        try:
            pc_agent.handle(self, method, parsed, self._client_ip())
        except Exception as e:
            try:
                self._send(500, json.dumps({"ok": False, "error": str(e)}).encode(), "application/json")
            except Exception:
                pass

    def do_POST(self):
        parsed = urlparse(self.path)
        if not self._peer_ok():
            return
        if parsed.path.startswith("/pc/"):
            return self._pc("POST", parsed)
        if not self._authorized():
            self._send(401, b'{"ok": false, "error": "unauthorized"}', "application/json")
            return
        if parsed.path == "/run":
            length = int(self.headers.get("Content-Length", 0))
            if length > 1_000_000:
                self._send(413, b'{"ok": false, "error": "too large"}', "application/json")
                return
            raw = self._read_body(length)
            if raw is None:
                self._send(400, b'{"ok": false, "error": "body hash mismatch"}', "application/json")
                return
            body = json.loads(raw or b"{}")
            cmd = body.get("cmd", "")
            timeout = body.get("timeout", 15)
            audit(self._client_ip(), "run", cmd)
            try:
                out = y5sh.run(cmd, timeout=timeout)
                self._send(200, json.dumps({"ok": True, "output": out}).encode(), "application/json")
            except Exception as e:
                self._send(500, json.dumps({"ok": False, "error": str(e)}).encode(), "application/json")
            return
        if parsed.path == "/push":
            qs = parse_qs(parsed.query)
            remote = qs.get("remote", [None])[0]
            if not remote:
                self._send(400, b'{"ok": false, "error": "missing remote"}', "application/json")
                return
            length = int(self.headers.get("Content-Length", 0))
            if length > MAX_BODY:
                self._send(413, b'{"ok": false, "error": "too large"}', "application/json")
                return
            audit(self._client_ip(), "push", "%s (%d B)" % (remote, length))
            data = self._read_body(length)
            if data is None:
                self._send(400, b'{"ok": false, "error": "body hash mismatch"}', "application/json")
                return
            port = alloc_port()
            try:
                push(data, remote, port)
                self._send(200, json.dumps({"ok": True, "bytes": len(data)}).encode(), "application/json")
            except Exception as e:
                self._send(500, json.dumps({"ok": False, "error": str(e)}).encode(), "application/json")
            return
        self._send(404, b'{"ok": false, "error": "not found"}', "application/json")

    def do_GET(self):
        parsed = urlparse(self.path)
        if not self._peer_ok():
            return
        if parsed.path.startswith("/pc/"):
            return self._pc("GET", parsed)
        if parsed.path != "/health" and not self._authorized():
            self._send(401, b'{"ok": false, "error": "unauthorized"}', "application/json")
            return
        if parsed.path == "/pull":
            qs = parse_qs(parsed.query)
            remote = qs.get("remote", [None])[0]
            if not remote:
                self._send(400, b'{"ok": false, "error": "missing remote"}', "application/json")
                return
            audit(self._client_ip(), "pull", remote)
            port = alloc_port()
            try:
                data = pull(remote, port)
                self._send(200, data)
            except Exception as e:
                self._send(500, json.dumps({"ok": False, "error": str(e)}).encode(), "application/json")
            return
        if parsed.path == "/health":
            self._send(200, b'{"ok": true}', "application/json")
            return
        self._send(404, b'{"ok": false, "error": "not found"}', "application/json")

    def log_message(self, fmt, *args):
        print("[bridge]", self.address_string(), fmt % args, flush=True)


if __name__ == "__main__":
    if len(TOKEN) < 64:
        sys.exit("BRIDGE_TOKEN must be set to a random string of at least 64 characters")
    if SIGN_KEY and len(SIGN_KEY) < 64:
        sys.exit("BRIDGE_SIGN_KEY must be at least 64 characters")
    if SIGN_KEY and hmac.compare_digest(SIGN_KEY.encode(), TOKEN.encode()):
        sys.exit("BRIDGE_SIGN_KEY must differ from BRIDGE_TOKEN")
    print("Request signing (phone endpoints):", "REQUIRED" if SIGN_KEY else "OFF (set BRIDGE_SIGN_KEY)", flush=True)
    if os.environ.get("BRIDGE_PS") == "1" and not pc_agent.ENABLED:
        sys.exit("BRIDGE_PS=1 requires BRIDGE_PS_TOKEN and BRIDGE_PS_SIGN_KEY of at least 100 characters each")
    if pc_agent.ENABLED:
        used = [s for s in (pc_agent.PS_TOKEN, pc_agent.PS_SIGN_KEY, TOKEN, SIGN_KEY) if s]
        if len(set(used)) != len(used):
            sys.exit("BRIDGE_PS_TOKEN, BRIDGE_PS_SIGN_KEY, BRIDGE_TOKEN and BRIDGE_SIGN_KEY must all differ")
    print("PC control (/pc/*):", "ENABLED - audit log: " + pc_agent.LOG_PATH if pc_agent.ENABLED else "disabled", flush=True)
    srv = ThreadingHTTPServer((BIND, LISTEN_PORT), Handler)
    print(f"bridge_server listening on {BIND}:{LISTEN_PORT}", flush=True)
    srv.serve_forever()
