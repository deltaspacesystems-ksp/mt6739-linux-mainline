#!/bin/bash
set -eu
# Keep source authority on D:, compile on ext4 to avoid slow Windows metadata I/O.
mkdir -p /home/y5/rogue-ddk19-port
cd /mnt/d/y5_linux_work/rogue-ddk19-port
find . -type f \( -name '*.c' -o -name '*.h' -o -name 'Makefile' -o -name '*.mk' \) -print0 |
  tar --null -T - -cf - | tar -xf - -C /home/y5/rogue-ddk19-port
