import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for p in wvkbd squeekboard sway labwc wayfire river hikari phoc wlr-randr foot-extra grim slurp swaybg waybar wofi fuzzel tofi; do
  r=$(apk search -x $p 2>/dev/null | head -n 1); printf '%-14s %s\n' $p "${r:-BRAK}"
done
""", timeout=60))
