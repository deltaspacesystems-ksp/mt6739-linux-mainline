#!/bin/bash
set -eu
r=/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19
flags=()
for dir in "$r"/generated/*; do [ ! -d "$dir" ] || flags+=("-I$dir"); done
cc -std=gnu11 -Wall -Wextra -Werror -include "$r/include/config_kernel_user_mt6739.h" -I"$r/include" -I"$r/include/public" -I"$r/services/include" -I"$r/services/include/shared" -I"$r/services/shared/include" -I"$r/hwdefs/km" -I/home/y5/linux-7.3/include/uapi -I/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19/hwdefs "${flags[@]}" -c /mnt/d/y5_linux_work/transfer_abi_assert.c -o /tmp/mfw-transfer-abi.o
printf 'PASS exact generated transfer ABI and disabled reset-framework size\n'
