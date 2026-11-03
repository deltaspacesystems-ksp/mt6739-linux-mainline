import sys, threading, time, socket
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def push(local, remote, port=2330, mode="755"):
    data = open(local, "rb").read()
    t = threading.Thread(target=lambda: y5sh.run("nc -l -p %d > %s; echo recv_done" % (port, remote), timeout=120)); t.start(); time.sleep(1.5)
    s = socket.create_connection(("172.16.42.1", port), timeout=5); s.sendall(data); s.close(); t.join(120)
    return y5sh.run("chmod %s %s; ls -l %s" % (mode, remote, remote), timeout=15)
if __name__ == "__main__":
    print(push(sys.argv[1], sys.argv[2]))
