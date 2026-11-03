cd /home/y5/linux-7.3
grep -n -B12 'phandle = <0x01>;' /mnt/d/y5_linux_work/out/stock_0.dts | grep -E 'compatible|^\S*[0-9]+-\s*[a-z_0-9@-]+: |phandle' | head -6
grep -E 'CONFIG_(IRQ_MTK_SYSIRQ|MTK_SYSIRQ|ARM_GIC_V3)=' .config
ls drivers/irqchip | grep -i -E 'mtk|sysirq'
grep -n 'compatible' drivers/irqchip/irq-mtk-sysirq.c | head -8
