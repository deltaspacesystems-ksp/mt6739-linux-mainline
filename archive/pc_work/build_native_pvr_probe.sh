#!/bin/bash
set -eu
src=/mnt/d/y5_linux_work/native_pvr_probe.c
ref=/home/y5/rogue-ddk19-port
flags=(-std=gnu11 -Wall -Wextra -Werror -include "$ref/include/config_kernel_user_mt6739.h"
 -I"$ref/include" -I"$ref/include/public" -I"$ref/services/include"
 -I"$ref/services/include/shared" -I"$ref/services/shared/include"
 -I"$ref/generated/srvcore_bridge" -I"$ref/generated/mm_bridge" -I/home/y5/linux-7.3/include/uapi/drm)
cc "${flags[@]}" "$src" -o /tmp/mfw-pvr-native-probe
/tmp/mfw-pvr-native-probe --abi
aarch64-linux-gnu-gcc "${flags[@]}" -static "$src" -o /mnt/d/y5_linux_work/out/mfw-pvr-native-probe
