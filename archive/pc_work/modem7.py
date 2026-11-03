import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
B=/dev/block; N=$B/platform/bootdevice/by-name
mkdir -p $N
for b in /sys/class/block/mmcblk0p*; do
  n=$(basename $b); MM=$(cat $b/dev); [ -e $B/$n ] || mknod $B/$n b ${MM%:*} ${MM#*:}
  pn=$(sed -n 's/^PARTNAME=//p' $b/uevent); [ -n "$pn" ] && ln -sf $B/$n $N/$pn
done
ls $N | wc -l; ls -l $N/nvdata
for p in nvdata:nvdata nvcfg:nvcfg protect1:protect_f protect2:protect_s; do
  part=${p%%:*}; mp=/aroot/vendor/${p##*:}
  mountpoint -q $mp || mount -t ext4 -o noatime,nosuid,nodev $N/$part $mp 2>&1
done
grep -E 'nvdata|nvcfg|protect' /proc/mounts
ls /aroot/vendor/nvdata | head; 
""", timeout=40))
