import http.server, socketserver, urllib.request, os, sys, threading
UP = "http://dl-cdn.alpinelinux.org"
CACHE = r"D:\y5_linux_work\apkcache"
os.makedirs(CACHE, exist_ok=True)
class H(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        path = self.path.split("?")[0]
        local = os.path.join(CACHE, path.lstrip("/").replace("/", os.sep))
        try:
            if not os.path.exists(local):
                os.makedirs(os.path.dirname(local), exist_ok=True)
                with urllib.request.urlopen(UP + path, timeout=60) as r, open(local + ".part", "wb") as f:
                    while True:
                        b = r.read(1 << 16)
                        if not b: break
                        f.write(b)
                os.replace(local + ".part", local)
            data = open(local, "rb").read()
            self.send_response(200); self.send_header("Content-Length", str(len(data))); self.end_headers(); self.wfile.write(data)
            print("OK  ", path, len(data), flush=True)
        except Exception as e:
            print("ERR ", path, e, flush=True)
            try: self.send_error(404)
            except Exception: pass
    def log_message(self, *a): pass
class S(socketserver.ThreadingMixIn, http.server.HTTPServer): daemon_threads = True
S(("172.16.42.2", 8080), H).serve_forever()
