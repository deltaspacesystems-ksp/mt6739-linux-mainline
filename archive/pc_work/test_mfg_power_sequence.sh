#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work/microfw-gpu
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_mfg_power.c -o /tmp/mfw-test-mfg-power
/tmp/mfw-test-mfg-power
