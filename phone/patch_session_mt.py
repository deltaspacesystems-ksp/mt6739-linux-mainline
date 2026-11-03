import shutil
p = "/usr/local/bin/y5-session"
shutil.copy(p, p + ".legacy")
s = open(p).read()
if "labwc-mt" in s:
    raise SystemExit("already patched")

# 1) stary most dotyku (XTest, jeden punkt) zastepuje libinput - fbbridge nie jest juz potrzebny
s = s.replace("GPUPRESENT=1 python3 /usr/local/bin/fbbridge.py > /tmp/fbbridge.log 2>&1 &\n", "", 1)

# 2) po starcie tlumacza dotyku: baza udev + seatd bez VT
old = 'export DISPLAY=:0 WLR_BACKENDS=x11 WLR_RENDERER=pixman WLR_NO_HARDWARE_CURSORS=1\n'
assert old in s
new = '''sh /usr/local/bin/y5-udev-prep.sh
pkill seatd; sleep 0.5; rm -f /run/seatd.sock
(SEATD_VTBOUND=0 setsid seatd -g root > /tmp/seatd.log 2>&1 < /dev/null &)
for i in 1 2 3 4 5; do [ -S /run/seatd.sock ] && break; sleep 1; done
mkdir -p /root/.config/labwc-mt && : > /root/.config/labwc-mt/autostart
export DISPLAY=:0 WLR_BACKENDS=x11,libinput LIBSEAT_BACKEND=seatd WLR_RENDERER=pixman WLR_NO_HARDWARE_CURSORS=1
'''
s = s.replace(old, new, 1)
assert "labwc > /tmp/labwc.log 2>&1 &" in s
s = s.replace("labwc > /tmp/labwc.log 2>&1 &", "labwc -C /root/.config/labwc-mt > /tmp/labwc.log 2>&1 &", 1)
s = s.replace("# Sesja \"telefonowa\": Xvfb -> most fb0/dotyk -> labwc (Wayland, wlroots x11) + waybar + wvkbd.",
              "# Sesja \"telefonowa\": Xvfb -> gpupresent (obraz) ; dotyk: y5-touchb (uinput, typ B) -> libinput -> labwc (wlroots x11,libinput).", 1)
open(p, "w").write(s)
print("patched")
