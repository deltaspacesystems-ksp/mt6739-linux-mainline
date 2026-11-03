#!/bin/bash
set -eu
cd /home/y5/linux-7.3
echo MT6357_CODEC
grep -nE 'platform_driver|compatible|regmap|capture|Capture|MICBIAS|Mic Bias|SND_SOC_DAPM_ADC|supplies|snd_soc_dai_driver' sound/soc/codecs/mt6357.c | head -65 || true
echo MT6357_MFD
grep -nE 'sound|codec|mt6357' drivers/mfd/mt6397-core.c | head -35 || true
echo MEDIATEK_CAPTURE_DRIVERS
find drivers/media/platform/mediatek -maxdepth 2 -type f -name '*.[ch]' | head -30 || true
echo GNSS
grep -RnE 'mt6739|mediatek|wmt|stp' drivers/gnss || true
