import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\vktest", "/aroot/data/vktest", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/vktest
for c in red green; do echo 255 > /sys/class/leds/$c/brightness; sleep 1; echo 0 > /sys/class/leds/$c/brightness; done; echo leds_blinked
timeout 20 chroot /aroot /data/vktest 2>&1 </dev/null | tail -n 12
dmesg | grep -E 'PVR_K.*Error' | tail -n 3
""", timeout=60))
