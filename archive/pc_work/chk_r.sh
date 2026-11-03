cd /home/y5/linux-7.3
grep -E "CONFIG_(REGULATOR|REGULATOR_FIXED_VOLTAGE|REGULATOR_MT6.*|MMC_MTK|USB_ETH_RNDIS|USB_ETH|USB_G_ETHER|USB_GADGET)[= ]" .config
grep -n "regulator not available" drivers/mmc/host/*.c drivers/mmc/core/*.c | head
