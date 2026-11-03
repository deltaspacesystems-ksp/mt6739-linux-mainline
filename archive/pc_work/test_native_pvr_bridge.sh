#!/bin/bash
set -eu
ref=/home/y5/rogue-ddk19-port
cd /mnt/d/y5_linux_work
cc -std=gnu11 -Wall -Wextra -Werror -fsanitize=address,undefined \
 -include "$ref/include/config_kernel_user_mt6739.h" \
 -I"$ref/include" -I"$ref/include/public" -I"$ref/services/include" \
 -I"$ref/services/include/shared" -I"$ref/services/shared/include" \
 -I"$ref/generated/srvcore_bridge" -I"$ref/generated/mm_bridge" \
 -I/home/y5/linux-7.3/include/uapi/drm test_native_pvr_bridge.c -o /tmp/mfw-native-bridge-test
/tmp/mfw-native-bridge-test > /mnt/d/y5_linux_work/out/native-bridge-test.log 2>&1
tail -n 1 /mnt/d/y5_linux_work/out/native-bridge-test.log
