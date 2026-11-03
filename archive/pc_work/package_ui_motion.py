from pathlib import Path
import tarfile
r=Path('X:/y5linux/phone/ui')
with tarfile.open('D:/y5_linux_work/out/ui-motion-update.tar.gz','w:gz') as t:
 for n in ['src/motion.c','src/motion.h','src/render.c','src/desktop.c','src/wayland.c','Makefile']:t.add(r/n,arcname=n)
