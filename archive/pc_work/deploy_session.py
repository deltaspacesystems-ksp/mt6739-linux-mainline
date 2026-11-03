import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
G = r"D:\y5_linux_work\guisrc" + "\\"
rd = lambda n: open(G + n, encoding="utf-8").read()
R = "/mnt/root"
print(y5put.put(R + "/usr/local/bin/y5-session", rd("y5-session.sh"), "755"))
print(y5put.put(R + "/root/.config/labwc/autostart", rd("labwc-autostart"), "755"))
print(y5put.put(R + "/root/.config/waybar/config", rd("waybar-config.json"), "644"))
print(y5put.put(R + "/root/.config/waybar/style.css", """* { font-family: 'DejaVu Sans'; font-size: 22px; color: #cdd6f4; }
window#waybar { background: #181825; }
#custom-term, #custom-web, #custom-launch, #custom-kbd { background: #313244; border-radius: 10px; margin: 6px 4px; padding: 0 10px; }
#clock, #battery, #custom-wifi { padding: 0 12px; }
""", "644"))
print(y5put.put("/usr/local/bin/startgui3", """#!/bin/sh
/usr/local/bin/chroot-up >/dev/null
chroot /mnt/root /usr/local/bin/y5-session
""", "755"))
print(y5sh.run("/usr/local/bin/startgui3", timeout=60))
time.sleep(6)
print(y5sh.run("chroot /mnt/root sh -c 'ps | grep -E \"labwc|waybar|wvkbd|Xvfb|bridge\" | grep -v grep; tail -n 4 /tmp/labwc.log /tmp/waybar.log /tmp/wvkbd.log'", timeout=30))
