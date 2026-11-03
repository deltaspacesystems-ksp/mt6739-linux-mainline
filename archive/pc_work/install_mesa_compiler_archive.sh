#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/cache_mesa_compiler_archive.py
mapfile -t packages < /mnt/d/y5_linux_work/out/mesa-compiler-package-paths.txt
pacman -U --needed --noconfirm "${packages[@]}" > /mnt/d/y5_linux_work/out/mesa-compiler-install.log 2>&1
bash /mnt/d/y5_linux_work/build_mesa_native_baseline.sh
