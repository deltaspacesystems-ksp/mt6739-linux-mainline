import sys, socket, threading, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def push(local, remote, port):
    data = open(local, 'rb').read()
    def listener():
        y5sh.run('nc -l -p %d > %s; echo recv_done' % (port, remote), timeout=20)
    t = threading.Thread(target=listener)
    t.start()
    time.sleep(3)
    s = socket.create_connection(('172.16.42.1', port), timeout=8)
    s.sendall(data); s.close()
    t.join(20)
    return y5sh.run('chmod 755 %s' % remote, timeout=10)

files = [
    (r'D:\y5_linux_work\guisrc\icons.py', '/mnt/root/root/icons.py', 2750),
    (r'D:\y5_linux_work\guisrc\glass.py', '/mnt/root/root/glass.py', 2751),
    (r'D:\y5_linux_work\guisrc\status_bar.py', '/mnt/root/root/status_bar.py', 2752),
    (r'D:\y5_linux_work\guisrc\control_center.py', '/mnt/root/root/control_center.py', 2753),
]
for local, remote, port in files:
    push(local, remote, port)
    print("pushed", remote)
