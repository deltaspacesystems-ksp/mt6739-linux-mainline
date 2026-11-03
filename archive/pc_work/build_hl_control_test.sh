#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
aarch64-linux-gnu-gcc -static -O2 -Wall -Wextra -Werror -o out/y5_hl_control_test y5_hl_control_test.c
gcc -O2 -Wall -Wextra -Werror -fsanitize=undefined -o /tmp/test_mfw_hl_policy test_mfw_hl_policy.c
/tmp/test_mfw_hl_policy