import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- test: ile symboli w ogole widzi parser dla wlan_drv_gen2:'
python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/wlan_drv_gen2.ko 2>&1 | wc -l
echo '--- pierwsze 5:'
python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/wlan_drv_gen2.ko 2>&1 | head -n 5
echo '--- szukaj substring w kazdym .ko (bez filtra na calosc, potem grep):'
for f in wmt_chrdev_wifi wmt_drv wlan_drv_gen2; do
  echo "== $f =="
  python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/$f.ko 2>&1 | grep -E 'wifi_reset|p2p_mode'
done
""", timeout=40))
