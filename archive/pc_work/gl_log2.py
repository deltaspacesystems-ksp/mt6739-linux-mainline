import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
code = r'''
import socket,os,time,subprocess
os.makedirs("/aroot/dev/socket",exist_ok=True)
p="/aroot/dev/socket/logdw"
try: os.unlink(p)
except: pass
s=socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM); s.bind(p); os.chmod(p,0o666); s.settimeout(0.5)
pr=subprocess.Popen(["/aroot/data/gltest2"],stdout=open("/tmp/gl5.out","w"),stderr=subprocess.STDOUT,stdin=subprocess.DEVNULL)
t=time.time()
while time.time()-t<12:
    try:
        d=s.recv(4096); print(d[1:].replace(b"\0",b" | ")[:300])
    except socket.timeout: pass
pr.kill()
'''
import y5put
y5put.put("/mnt/root/tmp/lg.py", code)
print(y5sh.run("mkdir -p /mnt/root/aroot; mountpoint -q /mnt/root/aroot || mount --rbind /aroot /mnt/root/aroot; chroot /mnt/root python3 /tmp/lg.py 2>&1 | tail -n 40; tail -n 3 /mnt/root/tmp/gl5.out", timeout=60))
