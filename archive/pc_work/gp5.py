import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root sh -c 'export DISPLAY=:0; i=0; while [ $i -lt 40 ]; do xdotool mousemove $((50+i*12)) $((300+i*15)); i=$((i+1)); sleep 0.1; done'
sleep 2; tail -n 2 /tmp/gpup.out
""", timeout=60))
