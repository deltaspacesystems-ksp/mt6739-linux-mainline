#!/bin/bash
set -eu
cd /home/y5/linux-7.3
printf 'Exact MT6739 matches in radio/remoteproc trees:\n'
grep -R -n 'mt6739' drivers/bluetooth drivers/net/wireless/mediatek drivers/net/wwan drivers/remoteproc 2>/dev/null || true
printf 'BT UART compatibles/transports:\n'
grep -nE 'compatible =|serdev|STP|stp' drivers/bluetooth/btmtkuart.c | tail -30
printf 'T7xx PCI binding:\n'
grep -nE 'PCI_DEVICE|PCI_VENDOR|pci_driver|T700' drivers/net/wwan/t7xx/t7xx_pci.c | tail -14
printf 'Clock init symbols available:\n'
grep -nE 'PWRAP_WACS2_CMD|PWRAP_WACS2_RDATA|PWRAP_WACS2_VLDCLR' drivers/soc/mediatek/mtk-pmic-wrap.c | head -8
