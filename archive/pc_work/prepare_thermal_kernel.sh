#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
gcc -O2 -Wall -Wextra -Werror -fsanitize=undefined -o /tmp/test_y5_ntc test_y5_ntc.c
/tmp/test_y5_ntc
cd /home/y5/linux-7.3
cp /mnt/d/y5_linux_work/y5-pmic-monitor.c drivers/power/supply/
cp /mnt/d/y5_linux_work/y5-ntc.h drivers/power/supply/
