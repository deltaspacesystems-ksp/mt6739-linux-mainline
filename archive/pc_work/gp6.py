import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gpupresent", "/aroot/data/gpupresent.new", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gpupresent.new; mv /aroot/data/gpupresent.new /aroot/data/gpupresent; pkill gpupresent; sleep 4
chroot /mnt/root sh -c 'export DISPLAY=:0; i=0; while [ $i -lt 80 ]; do xdotool mousemove $((50+(i%40)*12)) $((300+(i%40)*15)); i=$((i+1)); sleep 0.02; done'
tail -n 3 /tmp/gpup.out
""", timeout=80))
