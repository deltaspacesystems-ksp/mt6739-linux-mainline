import sys, socket, threading, time, os
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

print(y5sh.run('mkdir -p /mnt/root/usr/share/fonts/sf', timeout=10))

fonts_dir = r"D:\y5_linux_work\fonts_extract\sfpro_final\Library\Fonts"
port = 2800
# just the weights we actually use: Regular, Medium, Semibold, Bold, Text + Display variants
wanted = [
    "SF-Pro-Text-Regular.otf", "SF-Pro-Text-Medium.otf", "SF-Pro-Text-Semibold.otf", "SF-Pro-Text-Bold.otf",
    "SF-Pro-Display-Regular.otf", "SF-Pro-Display-Medium.otf", "SF-Pro-Display-Semibold.otf", "SF-Pro-Display-Bold.otf",
]
for fname in wanted:
    local = os.path.join(fonts_dir, fname)
    remote = "/mnt/root/usr/share/fonts/sf/" + fname
    push(local, remote, port)
    print("pushed", fname)
    port += 1

mono_dir = r"D:\y5_linux_work\fonts_extract\sfmono_final\Library\Fonts"
for fname in ["SF-Mono-Regular.otf", "SF-Mono-Semibold.otf"]:
    local = os.path.join(mono_dir, fname)
    remote = "/mnt/root/usr/share/fonts/sf/" + fname
    push(local, remote, port)
    print("pushed", fname)
    port += 1

print(y5sh.run('chroot /mnt/root fc-cache -f /usr/share/fonts/sf 2>&1', timeout=20))
print(y5sh.run('chroot /mnt/root fc-list | grep -i "sf pro\\|sf mono"', timeout=10))
