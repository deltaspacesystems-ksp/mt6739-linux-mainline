"""Klient sesji chmurowej: telefon (przez bridge_server.py) i - gdy wlaczone - PC (/pc/*), z podpisywaniem zadan.

Telefon:  BRIDGE_URL=https://domena/y5-<losowy>   BRIDGE_TOKEN=...   BRIDGE_SIGN_KEY=...
PC:       PC_URL=https://domena/pc-<losowy>       BRIDGE_PS_TOKEN=... BRIDGE_PS_SIGN_KEY=...
Opcjonalnie Cloudflare Access: CF_ACCESS_CLIENT_ID / CF_ACCESS_CLIENT_SECRET.

Kazde zadanie niesie naglowki: Authorization (Bearer, sprawdza tez nginx) oraz podpis HMAC-SHA256
    X-Y5-Sig = HMAC(klucz_podpisu, czas \n nonce \n METODA \n sciezka?query \n sha256(tresci))
Podpisywana jest sciezka tak, jak widzi ja bridge_server.py (po zdjeciu przez nginx prefiksu /y5-.../ albo /pc-.../).
Klucz podpisu nigdy nie jest wysylany.

CLI:  python3 y5sh_cloud.py run "uname -a" | push LOKALNY ZDALNY | pull ZDALNY LOKALNY
      python3 y5sh_cloud.py ps "Get-Date" | job "polecenie" | job-status ID | pc-put LOK ZDALNA_ABS | pc-get ZDALNA_ABS LOK
"""
import os, json, time, hmac, hashlib, secrets, urllib.request, urllib.error
from urllib.parse import quote

UA = "y5-bridge-client/1.1"      # Cloudflare blokuje domyslny Python-urllib


class Scope:
    def __init__(self, base, token, key, prefix):
        self.base, self.token, self.key, self.prefix = base.rstrip("/"), token, key, prefix

    def request(self, method, rest, body=b"", content_type=None, timeout=60):
        """rest = czesc sciezki po prefiksie, np. 'run' albo 'push?remote=/tmp/x'. Zwraca (status, bajty)."""
        bsha = hashlib.sha256(body).hexdigest()
        ts, nonce = str(int(time.time())), secrets.token_hex(16)
        signed_path = self.prefix + rest
        msg = "\n".join([ts, nonce, method, signed_path, bsha]).encode()
        headers = {"Authorization": "Bearer " + self.token, "User-Agent": UA, "X-Y5-Time": ts, "X-Y5-Nonce": nonce,
                   "X-Y5-Body-Sha256": bsha,
                   "X-Y5-Sig": hmac.new(self.key.encode(), msg, hashlib.sha256).hexdigest()}
        if content_type:
            headers["Content-Type"] = content_type
        if os.environ.get("CF_ACCESS_CLIENT_ID"):
            headers["CF-Access-Client-Id"] = os.environ["CF_ACCESS_CLIENT_ID"]
            headers["CF-Access-Client-Secret"] = os.environ["CF_ACCESS_CLIENT_SECRET"]
        req = urllib.request.Request(self.base + "/" + rest, data=body if method == "POST" else None,
                                     method=method, headers=headers)
        try:
            with urllib.request.urlopen(req, timeout=timeout) as resp:
                return resp.status, resp.read()
        except urllib.error.HTTPError as e:
            return e.code, e.read()

    def json(self, method, rest, obj=None, timeout=60):
        body = json.dumps(obj).encode() if obj is not None else b""
        status, data = self.request(method, rest, body, "application/json" if obj is not None else None, timeout)
        try:
            out = json.loads(data)
        except ValueError:
            raise RuntimeError("HTTP %d: %r" % (status, data[:200]))
        if not out.get("ok"):
            raise RuntimeError(out.get("error", "HTTP %d" % status))
        return out


_phone = _pc = None


def phone():
    global _phone
    if _phone is None:
        _phone = Scope(os.environ["BRIDGE_URL"], os.environ["BRIDGE_TOKEN"], os.environ["BRIDGE_SIGN_KEY"], "/")
    return _phone


def pc():
    global _pc
    if _pc is None:
        _pc = Scope(os.environ["PC_URL"], os.environ["BRIDGE_PS_TOKEN"], os.environ["BRIDGE_PS_SIGN_KEY"], "/pc/")
    return _pc


# ---- telefon ---------------------------------------------------------------------------------------------------
def run(cmd, timeout=15):
    """Polecenie na telefonie (zewnetrzny root), jak y5sh.run()."""
    return phone().json("POST", "run", {"cmd": cmd, "timeout": timeout}, timeout + 15)["output"]


def push(local_path, remote_path):
    status, data = phone().request("POST", "push?remote=" + quote(remote_path, safe="/"), open(local_path, "rb").read(),
                                   "application/octet-stream", 90)
    out = json.loads(data)
    if not out.get("ok"):
        raise RuntimeError(out.get("error", "push failed"))
    return out


def pull(remote_path, local_path):
    status, data = phone().request("GET", "pull?remote=" + quote(remote_path, safe="/"), b"", None, 90)
    if status != 200:
        raise RuntimeError("pull failed: HTTP %d %r" % (status, data[:200]))
    open(local_path, "wb").write(data)
    return len(data)


# ---- PC (wymaga BRIDGE_PS=1 na PC) -----------------------------------------------------------------------------
def pc_ps(cmd, timeout=60):
    """Synchroniczne polecenie PowerShell na PC (max ~90 s przez Cloudflare; dluzsze -> pc_job)."""
    return pc().json("POST", "ps", {"cmd": cmd, "timeout": timeout}, timeout + 20)


def pc_job(cmd, timeout=3600):
    return pc().json("POST", "job", {"cmd": cmd, "timeout": timeout})["job"]


def pc_job_status(job_id, tail=20000):
    return pc().json("GET", "job?id=%s&tail=%d" % (job_id, tail))


def pc_kill(job_id):
    return pc().json("POST", "kill?id=" + job_id, {})


def pc_info():
    return pc().json("GET", "info")


def pc_put(local_path, remote_abs):
    status, data = pc().request("POST", "put?path=" + quote(remote_abs, safe=":/"), open(local_path, "rb").read(),
                                "application/octet-stream", 120)
    out = json.loads(data)
    if not out.get("ok"):
        raise RuntimeError(out.get("error", "put failed"))
    return out


def pc_get(remote_abs, local_path):
    status, data = pc().request("GET", "get?path=" + quote(remote_abs, safe=":/"), b"", None, 120)
    if status != 200:
        raise RuntimeError("get failed: HTTP %d %r" % (status, data[:200]))
    open(local_path, "wb").write(data)
    return len(data)


if __name__ == "__main__":
    import sys
    a = sys.argv[1:]
    cmds = {"run", "push", "pull", "ps", "job", "job-status", "pc-put", "pc-get", "pc-info"}
    if not a or a[0] not in cmds:
        print(run(" ".join(a) or "uname -a"))
    elif a[0] == "run":
        print(run(" ".join(a[1:])))
    elif a[0] == "push":
        print(push(a[1], a[2]))
    elif a[0] == "pull":
        print(pull(a[1], a[2]), "B")
    elif a[0] == "ps":
        r = pc_ps(" ".join(a[1:]))
        print(r["output"], end="")
        print("[exit %s%s]" % (r["exit"], ", timeout" if r["timed_out"] else ""))
    elif a[0] == "job":
        print(pc_job(" ".join(a[1:])))
    elif a[0] == "job-status":
        r = pc_job_status(a[1])
        print(r["output"], end="")
        print("[running=%s exit=%s %ss]" % (r["running"], r["exit"], r["seconds"]))
    elif a[0] == "pc-put":
        print(pc_put(a[1], a[2]))
    elif a[0] == "pc-get":
        print(pc_get(a[1], a[2]), "B")
    elif a[0] == "pc-info":
        print(pc_info())
