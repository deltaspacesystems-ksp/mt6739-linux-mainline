"""Zdalne sterowanie PC (PowerShell + pliki) dla sesji chmurowej Claude. DOMYSLNIE WYLACZONE.

Wlaczenie (w srodowisku procesu bridge_server.py):
    BRIDGE_PS=1
    BRIDGE_PS_TOKEN=<min. 100 znakow, INNY niz BRIDGE_TOKEN>
Zabezpieczenia: osobny token (token do telefonu tu nie dziala i odwrotnie), kazde wywolanie trafia do dziennika
ps_audit.log (czas, IP, operacja, polecenie, kod wyjscia), limit rozmiaru wyjscia i czasu, polecenia dzialaja z
uprawnieniami uzytkownika, ktory uruchomil bridge (nic nie podnosi uprawnien). Wylaczenie = zamkniecie procesu.
"""
import os, json, time, base64, hashlib, subprocess, threading, uuid, socket, getpass
from urllib.parse import parse_qs

HERE = os.path.dirname(os.path.abspath(__file__))
PS_TOKEN = os.environ.get("BRIDGE_PS_TOKEN", "")
PS_SIGN_KEY = os.environ.get("BRIDGE_PS_SIGN_KEY", "")          # podpis zadan /pc/* jest OBOWIAZKOWY
ENABLED = os.environ.get("BRIDGE_PS") == "1" and len(PS_TOKEN) >= 100 and len(PS_SIGN_KEY) >= 100
LOG_PATH = os.environ.get("BRIDGE_PS_LOG", os.path.join(HERE, "ps_audit.log"))
JOBS_DIR = os.path.join(HERE, "ps_jobs")
MAX_SYNC_OUT = 1_000_000          # bajtow wyjscia zwracanych w jednym wywolaniu
MAX_BODY = 1_000_000              # limit JSON w zadaniu
CHUNK = 1 << 20

PRELUDE = ("$ProgressPreference='SilentlyContinue'; $ErrorActionPreference='Continue'; "
           "[Console]::OutputEncoding=[Text.Encoding]::UTF8; ")
NOWIN = 0x08000000 if os.name == "nt" else 0       # CREATE_NO_WINDOW

JOBS = {}
LOCK = threading.Lock()

STARTED = time.time()
TTL = int(os.environ.get("BRIDGE_PS_TTL_MIN", "240")) * 60      # sterowanie PC wygasa samo po tylu minutach od startu
KILL_FILE = os.path.join(HERE, "DISABLE_PC")                    # wyłącznik awaryjny: utworz ten plik => /pc/* przestaje dzialac


def active():
    return ENABLED and (time.time() - STARTED) < TTL and not os.path.exists(KILL_FILE)


def audit(ip, op, detail, **extra):
    rec = {"t": time.strftime("%Y-%m-%d %H:%M:%S"), "ip": ip, "op": op, "detail": str(detail)[:2000]}
    rec.update(extra)
    with LOCK:
        with open(LOG_PATH, "a", encoding="utf-8") as f:
            f.write(json.dumps(rec, ensure_ascii=False) + "\n")


def _spawn(cmd, out):
    enc = base64.b64encode((PRELUDE + cmd).encode("utf-16le")).decode()
    return subprocess.Popen(
        ["powershell.exe", "-NoProfile", "-NonInteractive", "-ExecutionPolicy", "Bypass", "-EncodedCommand", enc],
        stdin=subprocess.DEVNULL, stdout=out, stderr=subprocess.STDOUT, creationflags=NOWIN)


def _kill_tree(p):
    try:
        subprocess.run(["taskkill", "/F", "/T", "/PID", str(p.pid)], capture_output=True, creationflags=NOWIN)
    except Exception:
        pass
    try:
        p.kill()
    except Exception:
        pass


def _read_tail(path, limit):
    size = os.path.getsize(path)
    with open(path, "rb") as f:
        if size > limit:
            f.seek(size - limit)
        data = f.read(limit)
    return data.decode("utf-8", "replace"), size


def run_sync(cmd, timeout):
    os.makedirs(JOBS_DIR, exist_ok=True)
    path = os.path.join(JOBS_DIR, "sync_%s.log" % uuid.uuid4().hex[:10])
    timed_out = False
    with open(path, "wb") as out:
        p = _spawn(cmd, out)
        try:
            code = p.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            _kill_tree(p)
            p.wait()
            code, timed_out = -9, True
    text, size = _read_tail(path, MAX_SYNC_OUT)
    try:
        os.remove(path)
    except OSError:
        pass
    return code, text, timed_out, size > MAX_SYNC_OUT


def start_job(cmd, timeout):
    os.makedirs(JOBS_DIR, exist_ok=True)
    jid = uuid.uuid4().hex[:10]
    path = os.path.join(JOBS_DIR, "job_%s.log" % jid)
    out = open(path, "wb")
    p = _spawn(cmd, out)
    job = {"id": jid, "cmd": cmd[:500], "started": time.time(), "path": path, "proc": p, "exit": None, "timed_out": False}
    with LOCK:
        JOBS[jid] = job

    def waiter():
        try:
            code = p.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            _kill_tree(p)
            p.wait()
            code = -9
            job["timed_out"] = True
        out.close()
        job["exit"] = code
        job["finished"] = time.time()
        audit("-", "job_end", cmd[:300], job=jid, exit=code, seconds=round(time.time() - job["started"], 1))

    threading.Thread(target=waiter, daemon=True).start()
    return jid


def _json(h, code, obj):
    h._send(code, json.dumps(obj, ensure_ascii=False).encode("utf-8"), "application/json")


def _body_json(h):
    n = int(h.headers.get("Content-Length", 0))
    if n > MAX_BODY:
        raise ValueError("body too large")
    raw = h._read_body(n)                   # sprawdza skrot tresci z podpisem
    if raw is None:
        raise ValueError("body hash mismatch")
    return json.loads(raw or b"{}")


def handle(h, method, parsed, ip):
    """h = instancja BaseHTTPRequestHandler z bridge_server (autoryzacja juz sprawdzona)."""
    path = parsed.path
    qs = parse_qs(parsed.query)

    if method == "GET" and path == "/pc/info":
        _json(h, 200, {"ok": True, "host": socket.gethostname(), "user": getpass.getuser(), "cwd": os.getcwd(),
                       "audit_log": LOG_PATH})
        return

    if method == "POST" and path == "/pc/ps":
        body = _body_json(h)
        cmd, timeout = body.get("cmd", ""), min(int(body.get("timeout", 60)), 3600)
        audit(ip, "ps", cmd)
        t0 = time.time()
        code, text, timed_out, truncated = run_sync(cmd, timeout)
        audit(ip, "ps_end", cmd[:200], exit=code, seconds=round(time.time() - t0, 1), timed_out=timed_out)
        _json(h, 200, {"ok": True, "exit": code, "output": text, "timed_out": timed_out, "truncated": truncated})
        return

    if method == "POST" and path == "/pc/job":
        body = _body_json(h)
        cmd, timeout = body.get("cmd", ""), min(int(body.get("timeout", 3600)), 24 * 3600)
        audit(ip, "job_start", cmd)
        _json(h, 200, {"ok": True, "job": start_job(cmd, timeout)})
        return

    if method == "GET" and path == "/pc/job":
        jid = (qs.get("id") or [""])[0]
        tail = min(int((qs.get("tail") or ["20000"])[0]), MAX_SYNC_OUT)
        job = JOBS.get(jid)
        if not job:
            _json(h, 404, {"ok": False, "error": "no such job"})
            return
        text, size = _read_tail(job["path"], tail)
        _json(h, 200, {"ok": True, "running": job["exit"] is None, "exit": job["exit"], "timed_out": job["timed_out"],
                       "seconds": round((job.get("finished") or time.time()) - job["started"], 1),
                       "output_bytes": size, "output": text})
        return

    if method == "POST" and path == "/pc/kill":
        jid = (qs.get("id") or [""])[0]
        job = JOBS.get(jid)
        if not job:
            _json(h, 404, {"ok": False, "error": "no such job"})
            return
        audit(ip, "kill", jid)
        _kill_tree(job["proc"])
        _json(h, 200, {"ok": True})
        return

    if method == "POST" and path == "/pc/put":
        dest = (qs.get("path") or [""])[0]
        if not dest or not os.path.isabs(dest):
            _json(h, 400, {"ok": False, "error": "path must be absolute"})
            return
        n = int(h.headers.get("Content-Length", 0))
        os.makedirs(os.path.dirname(dest), exist_ok=True)
        left = n
        sha = hashlib.sha256()
        with open(dest, "wb") as f:
            while left > 0:
                chunk = h.rfile.read(min(CHUNK, left))
                if not chunk:
                    break
                f.write(chunk)
                sha.update(chunk)
                left -= len(chunk)
        if left != 0 or (getattr(h, "_bsha", None) is not None and sha.hexdigest() != h._bsha):
            try:
                os.remove(dest)               # niepelne albo niezgodne z podpisem - nie zostawiamy pliku
            except OSError:
                pass
            audit(ip, "put_rejected", dest, bytes=n - left)
            _json(h, 400, {"ok": False, "error": "incomplete or hash mismatch"})
            return
        audit(ip, "put", dest, bytes=n)
        _json(h, 200, {"ok": True, "bytes": n})
        return

    if method == "GET" and path == "/pc/get":
        src = (qs.get("path") or [""])[0]
        if not src or not os.path.isabs(src) or not os.path.isfile(src):
            _json(h, 404, {"ok": False, "error": "no such file (absolute path required)"})
            return
        size = os.path.getsize(src)
        audit(ip, "get", src, bytes=size)
        h.send_response(200)
        h.send_header("Content-Type", "application/octet-stream")
        h.send_header("Content-Length", str(size))
        h.end_headers()
        with open(src, "rb") as f:
            while True:
                chunk = f.read(CHUNK)
                if not chunk:
                    break
                h.wfile.write(chunk)
        return

    _json(h, 404, {"ok": False, "error": "not found"})
