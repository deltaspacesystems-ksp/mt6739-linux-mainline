import sys, socket, threading, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def push(local, remote, port):
    data = open(local, 'rb').read()
    def listener():
        y5sh.run('nc -l -p %d > %s; echo recv_done' % (port, remote), timeout=30)
    t = threading.Thread(target=listener)
    t.start()
    time.sleep(3)
    s = socket.create_connection(('172.16.42.1', port), timeout=8)
    s.sendall(data); s.close()
    t.join(30)
    return y5sh.run('chmod 644 %s' % remote, timeout=10)

print(push(r'D:\y5_linux_work\guisrc\wallpaper.jpg', '/mnt/root/root/wallpaper.jpg', 2760))
print(push(r'D:\y5_linux_work\guisrc\icons.py', '/mnt/root/root/icons.py', 2761))
