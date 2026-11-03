import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
code = open(r"D:\y5_linux_work\gl_log3.py").read()
lg = r'''
import socket,os,time
p="/aroot/dev/socket/logdw"
os.makedirs("/aroot/dev/socket",exist_ok=True)
try: os.unlink(p)
except: pass
s=socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM); s.bind(p); os.chmod(p,0o666); s.settimeout(0.5)
t=time.time(); out=open("/tmp/logdw.txt","wb")
while time.time()-t<300:
    try:
        d=s.recv(4096); out.write(time.strftime("%H:%M:%S ").encode()+d[1:].replace(b"\0",b" | ")+b"\n"); out.flush()
    except socket.timeout: pass
'''
y5put.put("/mnt/root/tmp/lg.py", lg)
print(y5sh.run(r"""
pkill -f lg.py; ( chroot /mnt/root python3 /tmp/lg.py >/tmp/lg.err 2>&1 & ); sleep 1
mkdir -p /aroot/vendor/nvdata/md /aroot/data/ccci_cfg
E="PATH=/vendor/bin:/system/bin:/system/xbin"
( chroot /aroot /usr/bin/env $E /vendor/bin/nvram_daemon >/tmp/nvram.out 2>&1 </dev/null & ); sleep 4
( chroot /aroot /usr/bin/env $E /vendor/bin/ccci_fsd 0 >/tmp/fsd.out 2>&1 </dev/null & ); sleep 2
( chroot /aroot /usr/bin/env $E /vendor/bin/ccci_rpcd 0 >/tmp/rpcd.out 2>&1 </dev/null & ); sleep 2
ps | grep -E 'nvram|ccci' | grep -v grep
cat /tmp/nvram.out /tmp/fsd.out /tmp/rpcd.out | head -n 20
echo --- log; strings /mnt/root/tmp/logdw.txt | tail -n 25
""", timeout=60))
