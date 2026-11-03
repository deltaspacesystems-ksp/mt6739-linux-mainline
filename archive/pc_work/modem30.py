import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill ccci_mdinit; sleep 1; dmesg -c >/dev/null
LD_PRELOAD=/data/propshim.so timeout 50 /tmp/mstrace /vendor/bin/ccci_mdinit 0 >/dev/null 2>/tmp/mdtr2.txt </dev/null
grep -E 'c54\(|c322|c5\(' /tmp/mdtr2.txt | grep -v -E 'logdw|pmsg|/lib|ld.config|urandom' | head -n 80
echo ---; cat /sys/kernel/ccci/boot
""", timeout=90))
