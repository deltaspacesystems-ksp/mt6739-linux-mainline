import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /proc/device-tree
for n in msdc@11230000 usb0@11200000 apuart0@11002000 pwrap@1000d000; do
  echo "=== $n"
  for p in $n/*; do b=$(basename $p); [ -f $p ] || continue; sz=$(wc -c < $p); if [ $sz -le 96 ]; then printf "%s: " $b; (strings $p | head -c 80; echo; od -An -tx1 -v $p | tr -d '\n' | head -c 300; echo); fi; done | head -n 60
done
""", timeout=40))
