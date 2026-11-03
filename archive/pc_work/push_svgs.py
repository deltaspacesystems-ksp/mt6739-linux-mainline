import sys, socket, threading, time, os
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def push(local, remote, port):
    data = open(local, 'rb').read()
    def listener():
        y5sh.run('nc -l -p %d > %s; echo recv_done' % (port, remote), timeout=20)
    t = threading.Thread(target=listener)
    t.start()
    time.sleep(2.5)
    s = socket.create_connection(('172.16.42.1', port), timeout=8)
    s.sendall(data); s.close()
    t.join(20)
    return y5sh.run('chmod 644 %s' % remote, timeout=10)

print(y5sh.run('mkdir -p /mnt/root/root/symbols', timeout=10))

svg_dir = r"D:\y5_linux_work\guisrc\symbols"
port = 2800
for fname in sorted(os.listdir(svg_dir)):
    local = os.path.join(svg_dir, fname)
    remote = "/mnt/root/root/symbols/" + fname
    push(local, remote, port)
    print("pushed", fname)
    port += 1

print(y5sh.run('ls /mnt/root/root/symbols/', timeout=10))
