import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
R = "http://172.16.42.2:8080/alpine/v3.24"
script = r"""
cat > /tmp/stage2.sh <<'EOS'
#!/bin/sh
set -x
R=@R@
apk --root /mnt/root --keys-dir /etc/apk/keys --no-cache -X $R/main -X $R/community add \
  xorg-server xf86-video-fbdev xf86-input-evdev xinit xset xrandr xinput xdpyinfo setxkbmap \
  openbox st xterm svkbd dbus dbus-x11 font-dejavu mesa-dri-gallium \
  firefox-esr netsurf
echo STAGE2_RC=$?
du -sh /mnt/root | tail -n 1
echo STAGE2_DONE
EOS
chmod +x /tmp/stage2.sh
nohup /tmp/stage2.sh > /tmp/stage2.log 2>&1 &
echo started
""".replace("@R@", R)
print(y5sh.run(script, timeout=20))
last = ""
for i in range(120):
    time.sleep(15)
    out = y5sh.run("tail -n 2 /tmp/stage2.log; grep -c -E 'Installing|Downloading' /tmp/stage2.log; df -h /mnt/root | tail -n 1", timeout=15)
    if "STAGE2_DONE" in out or i % 6 == 5: print("[%ds] %s" % (i*15, out.strip().replace("\n", " | ")[:260]))
    if "STAGE2_DONE" in out: break
print(y5sh.run("grep -E 'STAGE2|ERROR|error:|fail|unable' /tmp/stage2.log | tail -n 8; df -h /mnt/root | tail -n 1; free -m | sed -n 2p", timeout=20))
