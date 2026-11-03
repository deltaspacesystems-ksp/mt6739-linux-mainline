#!/bin/bash
set -x
echo "=== START $(date)"
pacman -Syu --noconfirm --needed aarch64-linux-gnu-gcc bc flex bison git cpio openssl libelf pahole python-pyelftools dtc xz base-devel 2>&1 | tail -n 25
echo "=== pacman rc=$?"
mkdir -p /home/y5 && cd /home/y5
if [ ! -f linux-7.3.tar.xz ]; then
  curl -fL --retry 3 -o linux-7.3.tar.xz https://cdn.kernel.org/pub/linux/kernel/v7.x/linux-7.3.tar.xz
fi
ls -l linux-7.3.tar.xz
curl -fsL -o sha256sums.asc https://cdn.kernel.org/pub/linux/kernel/v7.x/sha256sums.asc
grep ' linux-7.3.tar.xz' sha256sums.asc
echo "lokalny: $(sha256sum linux-7.3.tar.xz)"
echo "=== DONE $(date)"