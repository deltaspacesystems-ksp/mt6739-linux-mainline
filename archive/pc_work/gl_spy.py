import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mountpoint -q /aroot/system || { mkdir -p /ro/system /ro/vendor /aroot/system /aroot/vendor /aroot/dev /aroot/proc /aroot/sys /aroot/data
 mountpoint -q /ro/vendor || mount -t ext4 -o ro /dev/part_vendor /ro/vendor
 mountpoint -q /ro/system || mount -t ext4 -o ro /dev/part_system /ro/system
 mount --rbind /ro/system /aroot/system; mount --rbind /ro/vendor /aroot/vendor; mount --rbind /dev /aroot/dev; mount --rbind /proc /aroot/proc; mount --rbind /sys /aroot/sys; }
ls /aroot/data/gltest >/dev/null 2>&1 || echo 'brak gltest w /aroot/data'
pkill -9 gltest 2>/dev/null
( timeout 20 chroot /aroot /data/gltest > /tmp/gl.out 2>&1 & )
sleep 4
P=$(pidof gltest | awk '{print $1}')
echo pid=$P
echo '--- syscall (nr + sp + pc):'; cat /proc/$P/syscall 2>&1
echo '--- wchan:'; cat /proc/$P/wchan 2>&1; echo
echo '--- stat (state, utime):'; cut -d' ' -f3,14,15 /proc/$P/stat
echo '--- otwarte fd:'; ls -l /proc/$P/fd 2>&1 | awk '{print $9,$10,$11}'
echo '--- maps (biblioteki):'; awk '{print $6}' /proc/$P/maps | sort -u | head -n 30
echo '--- out:'; cat /tmp/gl.out
""", timeout=40))
