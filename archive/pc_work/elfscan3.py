import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
script = y5sh.run("cat /usr/local/bin/ko_syms.py", timeout=15)
print(y5put.put("/mnt/root/usr/local/bin/ko_syms.py", script, "755"))
print(y5sh.run(r"""
for f in wmt_chrdev_wifi wmt_drv wlan_drv_gen2 bt_drv gps_drv fmradio_drv; do
  echo "== $f =="
  chroot /mnt/root python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/$f.ko wifi_reset_end wifi_reset_start register_set_p2p_mode_handler 2>&1
done
""", timeout=60))
