#!/bin/bash
ps aux | grep -c '[c]c1' ; ls /home/y5/linux-7.3/arch/arm64/boot/Image 2>&1
find /home/y5/linux-7.3 -name '*.o' -newer /home/y5/linux-7.3/.config 2>/dev/null | wc -l
