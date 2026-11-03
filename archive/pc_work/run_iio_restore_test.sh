#!/bin/bash
set -eu
cc -Wall -Wextra -Werror -Wno-unused-parameter -fsanitize=address,undefined /mnt/d/y5_linux_work/mfw-i2c1-probe/test_iio_restore.c -o /tmp/mfw-iio-restore-test
/tmp/mfw-iio-restore-test
