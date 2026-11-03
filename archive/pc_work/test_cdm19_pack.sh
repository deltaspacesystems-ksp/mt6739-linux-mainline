#!/bin/bash
set -eu
r=/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19
cc -Wall -Wextra -Werror -fsanitize=address,undefined -include "$r/include/config_kernel_user_mt6739.h" -I"$r/hwdefs/km" -I"$r/include" -I"$r/include/public" /mnt/d/y5_linux_work/test_cdm19_pack.c -o /tmp/mfw-test-cdm19-pack
/tmp/mfw-test-cdm19-pack
printf 'PASS 8177 common-size values, public field guards, PDS offset packing and rejection boundaries\n'
aarch64-linux-gnu-gcc -Wall -Wextra -Werror -include "$r/include/config_kernel_user_mt6739.h" -I"$r/hwdefs/km" -I"$r/include" -I"$r/include/public" -c /mnt/d/y5_linux_work/test_cdm19_pack.c -o /tmp/mfw-cdm19-pack-arm64.o
printf 'PASS ARM64 compile\n'
