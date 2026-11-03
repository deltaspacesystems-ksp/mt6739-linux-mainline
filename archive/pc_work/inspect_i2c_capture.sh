#!/bin/bash
set -eu
cd /home/y5/linux-7.3
printf 'I2C COMPATIBLES\n'
grep -nE 'compatible|static const struct mtk_i2c_compatible|mt673|mt676|mt2712' drivers/i2c/busses/i2c-mt65xx.c | tail -40
printf 'SENINF\n'
find drivers/media/platform/mediatek -iname '*seninf*' -o -iname '*camsv*'
printf 'AUDIO DAIS\n'
grep -nE 'capture =|stream_name|channels_min|channels_max|rate|of_match|compatible|dev_get_regmap|platform_get' sound/soc/codecs/mt6357.c | tail -30
