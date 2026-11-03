#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined \
  test_ddk19_dmabuf_fill.c -o /tmp/mfw-ddk19-fill-test
/tmp/mfw-ddk19-fill-test
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined \
  -D_POSIX_C_SOURCE=200809L test_ddk19_strings.c -o /tmp/mfw-ddk19-strings-test
/tmp/mfw-ddk19-strings-test
