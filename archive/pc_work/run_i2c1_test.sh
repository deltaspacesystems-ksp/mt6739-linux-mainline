#!/bin/bash
set -eu
cc -Wall -Wextra -Werror -fsanitize=address,undefined /mnt/d/y5_linux_work/mfw-i2c1-probe/test_allowlist.c -o /tmp/mfw-i2c1-test
/tmp/mfw-i2c1-test
