import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
R = "http://172.16.42.2:8080/alpine/v3.24"
script = r"""
cat > /tmp/stage3.sh <<'EOS'
#!/bin/sh
set -x
R=@R@
apk --root /mnt/root --keys-dir /etc/apk/keys --no-cache -X $R/main -X $R/community add \
  weston weston-terminal weston-backend-x11 cage wlroots foot wayland-utils wl-clipboard \
  mesa-dri-gallium mesa-egl mesa-gles seatd
echo STAGE3_RC=$?
du -sh /mnt/root | tail -n 1
echo STAGE3_DONE
EOS
chmod +x /tmp/stage3.sh
nohup /tmp/stage3.sh > /tmp/stage3.log 2>&1 &
echo started
""".replace("@R@", R)
print(y5sh.run(script, timeout=20))
for i in range(60):
    time.sleep(10)
    out = y5sh.run("tail -n 2 /tmp/stage3.log", timeout=15)
    if "STAGE3_DONE" in out: break
print(y5sh.run("grep -E 'STAGE3|ERROR|error:|unable|No such package' /tmp/stage3.log | tail -n 8; df -h /mnt/root | tail -n 1", timeout=20))
