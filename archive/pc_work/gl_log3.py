import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
code = r'''
import socket,os,time
p="/aroot/dev/socket/logdw"
os.makedirs("/aroot/dev/socket",exist_ok=True)
try: os.unlink(p)
except: pass
s=socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM); s.bind(p); os.chmod(p,0o666); s.settimeout(0.5)
t=time.time(); out=open("/tmp/logdw.txt","wb")
while time.time()-t<20:
    try:
        d=s.recv(4096); out.write(d[1:].replace(b"\0",b" | ")+b"\n"); out.flush()
    except socket.timeout: pass
'''
y5put.put("/mnt/root/tmp/lg.py", code)
print(y5sh.run("( chroot /mnt/root python3 /tmp/lg.py >/tmp/lg.err 2>&1 & ); sleep 2; ls -l /aroot/dev/socket; timeout 12 chroot /aroot /data/gltest2 >/dev/null 2>&1 </dev/null; sleep 1; cat /mnt/root/tmp/logdw.txt | tail -n 30; cat /tmp/lg.err", timeout=60))
