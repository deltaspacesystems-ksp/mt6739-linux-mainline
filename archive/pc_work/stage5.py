import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
R = "http://172.16.42.2:8080/alpine/v3.24"
script = r"""
cat > /tmp/stage5.sh <<'EOS'
#!/bin/sh
set -x
R=@R@
apk --root /mnt/root --keys-dir /etc/apk/keys --no-cache -X $R/main -X $R/community add labwc wvkbd fuzzel waybar swaybg wlr-randr grim font-awesome font-noto-emoji adwaita-icon-theme
echo STAGE5_RC=$?
echo STAGE5_DONE
EOS
chmod +x /tmp/stage5.sh
nohup /tmp/stage5.sh > /tmp/stage5.log 2>&1 &
echo started
""".replace("@R@", R)
print(y5sh.run(script, timeout=20))
for i in range(80):
    time.sleep(8)
    if "STAGE5_DONE" in y5sh.run("tail -n 2 /tmp/stage5.log", timeout=15): break
print(y5sh.run("grep -E 'STAGE5|ERROR|error:|unable' /tmp/stage5.log | tail -n 6; df -h /mnt/root | tail -n 1; chroot /mnt/root sh -c 'which labwc wvkbd-mobintl fuzzel waybar'", timeout=30))
