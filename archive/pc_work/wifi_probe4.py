import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- wszystkie moduly vendor:'; ls -la /ro/vendor/lib/modules/
echo '--- dmesg po probie wlan_drv_gen2 (ostatnie):'; dmesg | grep -i -A2 -B2 'wlan_drv_gen2\|unknown symbol' | tail -n 20
echo '--- symbole wymagane przez wlan_drv_gen2 (depmod-like, surowo):'; strings /ro/vendor/lib/modules/wlan_drv_gen2.ko | grep -E '^(cfg80211|wlan|mtk|wmt)_' | sort -u | head -n 40
echo '--- symbole eksportowane przez wmt_chrdev_wifi i wmt_drv:'; cat /proc/kallsyms | grep -i -E 'wmt_chrdev|^.{17} . wmt_' | wc -l
echo '--- aktualne symbole eksportowane (moduly):'; grep -F ' W1 ' /proc/kallsyms 2>/dev/null | head -n 3
grep ' [a-zA-Z] ' /proc/kallsyms | awk '{print $3}' | grep -E '^(wmt_|mtk_wcn)' | sort -u | head -n 30
""", timeout=40))
