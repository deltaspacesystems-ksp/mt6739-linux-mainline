"""TCP proxy: listens on this PC (reachable over Tailscale) and forwards to the phone's nc shell."""
import socket, threading, sys

LISTEN_HOST = "0.0.0.0"
LISTEN_PORT = 2323
TARGET_HOST = "172.16.42.1"
TARGET_PORT = 23

def pipe(src, dst):
    try:
        while True:
            data = src.recv(65536)
            if not data:
                break
            dst.sendall(data)
    except OSError:
        pass
    finally:
        try: dst.shutdown(socket.SHUT_WR)
        except OSError: pass

def handle(conn, addr):
    print("connection from", addr, flush=True)
    try:
        upstream = socket.create_connection((TARGET_HOST, TARGET_PORT), timeout=10)
    except OSError as e:
        print("upstream connect failed:", e, flush=True)
        conn.close()
        return
    t1 = threading.Thread(target=pipe, args=(conn, upstream), daemon=True)
    t2 = threading.Thread(target=pipe, args=(upstream, conn), daemon=True)
    t1.start(); t2.start()
    t1.join(); t2.join()
    conn.close(); upstream.close()
    print("connection closed", addr, flush=True)

def main():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind((LISTEN_HOST, LISTEN_PORT))
    srv.listen(8)
    print(f"tsproxy listening on {LISTEN_HOST}:{LISTEN_PORT} -> {TARGET_HOST}:{TARGET_PORT}", flush=True)
    while True:
        conn, addr = srv.accept()
        threading.Thread(target=handle, args=(conn, addr), daemon=True).start()

if __name__ == "__main__":
    main()
