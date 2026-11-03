import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- czy kallsyms zna te symbole:'; grep -E ' (wifi_reset_end|wifi_reset_start|register_set_p2p_mode_handler)$' /proc/kallsyms
echo '--- gdzie wystepuja jako T/t (definicja) w ktorym .ko (strings sam typ nie pokaze, sprawdzmy readelf symtab):'
for f in wmt_chrdev_wifi wmt_drv wlan_drv_gen2 bt_drv gps_drv fmradio_drv; do
  echo "== $f =="
  readelf -sW /ro/vendor/lib/modules/$f.ko 2>/dev/null | grep -E 'wifi_reset_end|wifi_reset_start|register_set_p2p_mode_handler'
done
echo '--- modinfo wmt_chrdev_wifi (parametry, zaleznosci):'; modinfo /ro/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1 | head -n 10
echo '--- rmmod i zaladuj ponownie wmt_chrdev_wifi z verbose:'; rmmod wmt_chrdev_wifi 2>&1; insmod /ro/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1; lsmod
""", timeout=40))
