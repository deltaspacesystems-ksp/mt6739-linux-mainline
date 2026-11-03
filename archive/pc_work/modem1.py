import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /dev ccci/tty/md'; ls -l /dev | grep -i -E 'ccci|ttyC|md|eemcs|ttyMT|mtk_md|ccmni|radio' | head -n 40
echo '--- /sys/class/misc ccci'; ls /sys/class/misc | grep -i -E 'ccci|md'
echo '--- /proc'; ls /proc | grep -i -E 'ccci|md|eemcs'; ls /proc/ccci* 2>&1 | head
echo '--- sys/kernel/ccci'; ls /sys/kernel/ccci 2>&1; cat /sys/kernel/ccci/boot 2>&1 | head -n 20
echo '--- net'; ls /sys/class/net
echo '--- dmesg ccci'; dmesg | grep -i -E 'ccci|md1|\[ccci' | head -n 30
echo '--- fw'; ls -l /vendor/firmware | grep -i -E 'md|modem|dsp|cert' | head -n 20
echo '--- partitions'; ls -l /dev/block/platform/*/by-name 2>/dev/null | grep -i -E 'md1|md3|nvram|nvdata|protect|nvcfg|modem|md_' | head
""", timeout=40))
