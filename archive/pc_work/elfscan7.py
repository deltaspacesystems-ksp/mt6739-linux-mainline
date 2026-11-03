import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/wmt_chrdev_wifi.ko __ksymtab_wifi_reset_end __ksymtab_wifi_reset_start __ksymtab_register_set_p2p_mode_handler 2>&1
echo '--- sekcje __ksymtab* w wmt_chrdev_wifi (grep po calym dumpie symboli):'
chroot /mnt/root python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1 | grep -c ksymtab
chroot /mnt/root python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1 | grep ksymtab | head -n 10
""", timeout=30))
