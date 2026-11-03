import sys, socket, threading, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def push(local, remote, port):
    data = open(local, 'rb').read()
    result = {}
    def listener():
        result['out'] = y5sh.run('nc -l -p %d > %s; echo recv_done' % (port, remote), timeout=20)
    t = threading.Thread(target=listener)
    t.start()
    time.sleep(3)
    s = socket.create_connection(('172.16.42.1', port), timeout=8)
    s.sendall(data); s.close()
    t.join(20)
    return y5sh.run('chmod 755 %s' % remote, timeout=10)

print(push(r'D:\y5_linux_work\guisrc\app_switcher.py', '/mnt/root/root/app_switcher.py', 2691))
print(push(r'D:\y5_linux_work\guisrc\homebar.py', '/mnt/root/root/homebar.py', 2692))
print(y5sh.run('ls -la /mnt/root/root/app_switcher.py /mnt/root/root/homebar.py', timeout=10))
