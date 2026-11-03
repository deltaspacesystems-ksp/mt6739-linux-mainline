import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
R = "http://172.16.42.2:8080/alpine/v3.24"
script = r"""
cat > /tmp/stage4.sh <<'EOS'
#!/bin/sh
set -x
R=@R@
apk --root /mnt/root --keys-dir /etc/apk/keys --no-cache -X $R/main -X $R/community add xvfb python3 py3-xlib xdotool xwd
echo STAGE4_RC=$?
echo STAGE4_DONE
EOS
chmod +x /tmp/stage4.sh
nohup /tmp/stage4.sh > /tmp/stage4.log 2>&1 &
echo started
""".replace("@R@", R)
print(y5sh.run(script, timeout=20))
for i in range(60):
    time.sleep(8)
    if "STAGE4_DONE" in y5sh.run("tail -n 2 /tmp/stage4.log", timeout=15): break
print(y5sh.run("grep -E 'STAGE4|ERROR|error:|unable' /tmp/stage4.log | tail -n 6; chroot /mnt/root sh -c 'python3 --version; which Xvfb weston cage firefox-esr'; df -h /mnt/root | tail -n 1", timeout=30))
