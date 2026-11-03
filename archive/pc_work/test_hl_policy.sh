#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
gcc -O2 -Wall -Wextra -Werror -fsanitize=undefined -o /tmp/test_mfw_hl_policy test_mfw_hl_policy.c
/tmp/test_mfw_hl_policy