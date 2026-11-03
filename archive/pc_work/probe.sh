#!/bin/bash
echo uname: $(uname -r)
echo user: $(whoami)
echo nproc: $(nproc)
free -g | sed -n 2p
df -h / | tail -n 1
for t in gcc make bc flex bison git python3 aarch64-linux-gnu-gcc clang ld.lld pahole dtc cpio xz; do printf '%-22s %s\n' $t "$(command -v $t || echo -)"; done
ping -c1 -W3 archlinux.org >/dev/null 2>&1 && echo net: ok || echo net: brak
