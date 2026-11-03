import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
R = "http://172.16.42.2:8080/alpine/v3.24"
script = r"""
cat > /tmp/stage1.sh <<'EOS'
#!/bin/sh
set -x
R=@R@
apk --root /mnt/root --initdb --keys-dir /etc/apk/keys --no-cache -X $R/main -X $R/community add \
  alpine-base alpine-conf openssh wpa_supplicant iw dhcpcd e2fsprogs util-linux \
  curl nano htop procps tzdata chrony font-dejavu ca-certificates libgcc musl-utils
echo STAGE1_RC=$?
du -sh /mnt/root | tail -n 1
echo STAGE1_DONE
EOS
chmod +x /tmp/stage1.sh
nohup /tmp/stage1.sh > /tmp/stage1.log 2>&1 &
echo started
""".replace("@R@", R)
print(y5sh.run(script, timeout=20))
for i in range(40):
    time.sleep(15)
    out = y5sh.run("tail -n 3 /tmp/stage1.log; grep -c Installing /tmp/stage1.log", timeout=15)
    if "STAGE1_DONE" in out or i % 4 == 3: print("[%ds] %s" % (i*15, out.strip().replace("\n", " | ")[:300]))
    if "STAGE1_DONE" in out: break
print(y5sh.run("grep -E 'STAGE1|ERROR|error|fail' /tmp/stage1.log | tail -n 8; df -h /mnt/root | tail -n 1; ls /mnt/root", timeout=20))
