"""HTTP relay for the cloud Claude session: run shell commands on the phone, push/pull files.
Reachable only via Tailscale (firewall rule scoped to the tailnet CGNAT range). No auth token,
by explicit user choice — anything in the tailnet can run commands on the phone through this.
"""
import sys, socket, threading, time, json
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

LISTEN_PORT = 2324
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


class Handler(BaseHTTPRequestHandler):
    def _send(self, code, body, content_type="application/octet-stream"):
        self.send_response(code)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_POST(self):
        parsed = urlparse(self.path)
        if parsed.path == "/run":
            length = int(self.headers.get("Content-Length", 0))
            body = json.loads(self.rfile.read(length) or b"{}")
            cmd = body.get("cmd", "")
            timeout = body.get("timeout", 15)
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
            data = self.rfile.read(length)
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
        if parsed.path == "/pull":
            qs = parse_qs(parsed.query)
            remote = qs.get("remote", [None])[0]
            if not remote:
                self._send(400, b'{"ok": false, "error": "missing remote"}', "application/json")
                return
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
    srv = ThreadingHTTPServer(("0.0.0.0", LISTEN_PORT), Handler)
    print(f"bridge_server listening on 0.0.0.0:{LISTEN_PORT}", flush=True)
    srv.serve_forever()
