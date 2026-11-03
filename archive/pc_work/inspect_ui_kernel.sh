#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -E 'CONFIG_(MEMFD|SHMEM|TMPFS|UNIX|EPOLL|EVENTFD|SIGNALFD|INPUT_MISC|INPUT_UINPUT|VT)' .config
