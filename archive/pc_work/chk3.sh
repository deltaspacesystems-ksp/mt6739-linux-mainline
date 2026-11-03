cd /home/y5/linux-7.3/drivers/mmc/host
sed -n '/^static int msdc_drv_probe/,/^host_free:/p' mtk-sd.c | grep -n -E 'devm_|of_|return|goto|dev_err|dev_dbg|get_|PTR_ERR|-E[A-Z]+' | head -90
grep -n 'mt8183-mmc\|mt6739\|mt6795' mtk-sd.c | head
