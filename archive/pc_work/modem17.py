import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
lg = r'''
import socket,os,time
p="/aroot/dev/socket/logdw"
os.makedirs("/aroot/dev/socket",exist_ok=True)
try: os.unlink(p)
except: pass
s=socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM); s.bind(p); os.chmod(p,0o666); s.settimeout(0.5)
t=time.time(); out=open("/tmp/logdw.txt","wb")
while time.time()-t<3000:
    try:
        d=s.recv(4096); out.write(time.strftime("%H:%M:%S ").encode()+d[1:].replace(b"\0",b" | ")+b"\n"); out.flush()
    except socket.timeout: pass
'''
y5put.put("/mnt/root/tmp/lg.py", lg)
print(y5sh.run(r"""
pkill -f lg.py; pkill ccci_mdinit; rm -f /mnt/root/tmp/logdw.txt
( chroot /mnt/root python3 /tmp/lg.py >/tmp/lg.err 2>&1 & ); sleep 1
( chroot /aroot /vendor/bin/ccci_mdinit 0 >/tmp/mdinit.out 2>&1 </dev/null & ); sleep 8
strings /mnt/root/tmp/logdw.txt | head -n 30
""", timeout=40))
