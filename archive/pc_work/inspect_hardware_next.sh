#!/bin/bash
set -eu
cd /home/y5/linux-7.3
printf '=== mainline block support ===\n'
find drivers/pinctrl/mediatek drivers/clk/mediatek -maxdepth 1 -type f -name '*6739*'
grep -nE 'mt6739|mt6765|mt6357' drivers/spi/spi-mt65xx.c drivers/soc/mediatek/mtk-pmic-wrap.c drivers/i2c/busses/i2c-mt65xx.c drivers/mfd/mt6357* drivers/iio/adc/mt6359-auxadc.c 2>/dev/null || true
printf '=== selected source files ===\n'
find drivers -type f \( -name '*mt6357*' -o -name '*mc32*' -o -name '*ltr*' -o -name '*sx93*' -o -name '*aw36*' -o -name '*goodix*' \)
printf '=== active config ===\n'
grep -E '^CONFIG_(I2C|PINCTRL|GPIO|MFD_MT|REGULATOR_MT|MTK_PMIC|TOUCHSCREEN_GOODIX|IIO|LEDS|COMMON_CLK)' .config
