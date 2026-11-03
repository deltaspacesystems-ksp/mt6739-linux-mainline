p = "/usr/local/bin/y5-session"
s = open(p).read()
if "y5-touchb" in s:
    raise SystemExit("already patched")
s = s.replace("pkill -f fbbridge.py; pkill labwc; pkill Xvfb\n",
              "pkill -f fbbridge.py; pkill -f y5-touchb.py; pkill labwc; pkill Xvfb\n", 1)
block = '''GPUPRESENT=1 python3 /usr/local/bin/fbbridge.py > /tmp/fbbridge.log 2>&1 &

# Prawdziwy multitouch: tlumacz protokolu A -> B przez uinput (wirtualne urzadzenie y5-touch-b).
python3 /usr/local/bin/y5-touchb.py > /tmp/touchb.log 2>&1 &
for i in 1 2 3 4 5 6 7 8; do
  sleep 1
  for d in /sys/class/input/event*; do
    if [ "$(cat $d/device/name 2>/dev/null)" = "y5-touch-b" ]; then
      n=$(basename $d); mm=$(cat $d/dev); mkdir -p /dev/input
      [ -e /dev/input/$n ] || mknod /dev/input/$n c ${mm%%:*} ${mm##*:}
      chmod 666 /dev/input/$n; ln -sf /dev/input/$n /dev/input/y5-touch-b; break 2
    fi
  done
done
'''
s = s.replace("GPUPRESENT=1 python3 /usr/local/bin/fbbridge.py > /tmp/fbbridge.log 2>&1 &\n", block, 1)
open(p, "w").write(s)
print("patched")
