#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py
cc -Wall -Wextra -Werror -fsanitize=address,undefined -I/usr/include/libdrm /mnt/d/y5_linux_work/test_mesa_ddk19_adapter.c -o /tmp/mfw-mesa19-test
/tmp/mfw-mesa19-test
