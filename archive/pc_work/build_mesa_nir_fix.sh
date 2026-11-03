#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/fix_mesa_nir_exports.py
cc -Wall -Wextra -Werror -fsanitize=address,undefined /mnt/d/y5_linux_work/test_mesa_nir_exports.c -o /tmp/mfw-mesa-nir-test
/tmp/mfw-mesa-nir-test
ninja -C /home/y5/mesa-native/build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-native-build-export-fix.log 2>&1
