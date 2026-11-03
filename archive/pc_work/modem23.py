import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /sys/kernel/ccci/boot
dmesg | grep -i -E 'md_state|ready|exception|EE|assert|handshake|fsm\]' | grep -v -E 'VcoreFS|charger' | tail -n 15
ls /sys/class/tty | grep -v -E '^(tty[0-9a-z]*|pty.*)$' | head -n 20
ls /sys/class/ccci_node | grep -i -E 'at|tty|ctrl|pipe|ccci_uem' | head
ls /dev/radio 2>&1 | head
""", timeout=40))
