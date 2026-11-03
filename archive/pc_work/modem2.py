import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /sys/kernel/ccci; for f in version md_en md1_postfix lk_md md_chn kcfg_setting ft_info; do echo "== $f"; cat $f 2>&1 | head -n 8; done
ls mdsys1; 
echo '--- classes'; ls /sys/class | grep -i -E 'ccci|ccmni|tty|radio'
echo '--- ccci devs'; grep -r . /sys/class/ccci*/*/dev 2>/dev/null | head -n 30; ls /sys/class/tty | grep -v -E '^tty[0-9]*$' | head -n 30
echo '--- parts'; for b in /sys/class/block/mmcblk0p*; do grep PARTNAME $b/uevent; done | grep -i -E 'md|nv|modem|protect|persist|proinfo' 
echo '--- cmdline'; cat /proc/cmdline | tr ' ' '\n' | grep -i -E 'md|ccci'
dmesg | grep -i -E 'ccci|\bmd\b|modem' | head -n 15
""", timeout=40))
