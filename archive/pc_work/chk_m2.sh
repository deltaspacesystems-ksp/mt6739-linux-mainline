cd /home/y5/linux-7.3/drivers/usb/musb
for l in 1848 2318 2362 2421 2507; do echo "=== $l"; sed -n "$((l-14)),$((l+1))p" musb_core.c; done
