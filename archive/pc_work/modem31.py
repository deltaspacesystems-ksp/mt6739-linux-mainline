import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
grep -E 'c54\(|c322|c5\(' /tmp/mdtr2.txt | grep -v -E 'logdw|pmsg|/lib|ld.config|urandom|prop.log' | sed -n '66,200p'
echo ---; grep -c . /tmp/mdtr2.txt; tail -n 3 /tmp/mdtr2.txt
echo --- md state now; cat /sys/kernel/ccci/boot
""", timeout=40))
