from pathlib import Path
w=Path(r'D:\y5_linux_work');s=(w/'export/y5-pmic-before-v59.c').read_text()
s=s.replace('#include <linux/workqueue.h>','#include <linux/workqueue.h>\n#include <linux/regulator/driver.h>\n#include <linux/regulator/machine.h>')
a=s.index('static int y5_adc_request(')
extra='''/* Read-only shared VCORE monitor. Linux7.3 MT6357 regulator driver uses
 * DBG0[6:0] for the active DA selector; ELR0 is only the programmed selector.
 * No enable/disable/set_voltage callbacks: CPU/VCORE is never altered here. */
static int y5_vcore_voltage(struct y5_pmic *p)
{
 unsigned int value;
 int ret;
 mutex_lock(&p->lock);
 ret = y5_read(p, 0x1522, &value);
 mutex_unlock(&p->lock);
 return ret ? ret : 518750 + (value & 127) * 6250;
}
static int y5_vcore_get_voltage(struct regulator_dev *rdev)
{
 return y5_vcore_voltage(rdev_get_drvdata(rdev));
}
static int y5_vcore_is_enabled(struct regulator_dev *rdev)
{
 struct y5_pmic *p = rdev_get_drvdata(rdev);
 unsigned int value;
 int ret;
 mutex_lock(&p->lock);
 ret = y5_read(p, 0x1508, &value);
 mutex_unlock(&p->lock);
 return ret ? ret : !!(value & 1);
}
static const struct regulator_ops y5_vcore_readonly_ops = {
 .get_voltage = y5_vcore_get_voltage,
 .is_enabled = y5_vcore_is_enabled,
 .list_voltage = regulator_list_voltage_linear,
};
static const struct regulator_desc y5_vcore_readonly_desc = {
 .name = "microfw-vcore-readonly", .type = REGULATOR_VOLTAGE,
 .owner = THIS_MODULE, .ops = &y5_vcore_readonly_ops,
 .n_voltages = 128, .min_uV = 518750, .uV_step = 6250,
};
static void y5_register_vcore_readonly(struct y5_pmic *p)
{
 struct device_node *np = of_get_child_by_name(p->dev->of_node, "vcore-readonly");
 struct regulator_init_data init = {
  .constraints = { .name = "microfw-vcore-readonly", .always_on = true,
   .boot_on = true, .min_uV = 518750, .max_uV = 1312500 },
 };
 struct regulator_config config = {
  .dev = p->dev, .driver_data = p, .init_data = &init,
 };
 struct regulator_dev *rdev;
 if (!np) return;
 config.of_node = np;
 rdev = devm_regulator_register(p->dev, &y5_vcore_readonly_desc, &config);
 of_node_put(np);
 if (IS_ERR(rdev))
  dev_warn(p->dev, "Optional VCORE readonly regulator unavailable: %ld\\n", PTR_ERR(rdev));
 else
  dev_info(p->dev, "VCORE readonly regulator registered; no voltage/enable writes\\n");
}
static ssize_t vcore_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
 struct y5_pmic *p = dev_get_drvdata(dev);
 unsigned int da, programmed, control;
 int ret;
 mutex_lock(&p->lock);
 ret = y5_read(p, 0x1522, &da);
 if (!ret) ret = y5_read(p, 0x152a, &programmed);
 if (!ret) ret = y5_read(p, 0x1508, &control);
 mutex_unlock(&p->lock);
 if (ret) return ret;
 return sysfs_emit(buf, "selector=%u programmed_selector=%u enabled=%u voltage_uv=%u\\n",
  da & 127, programmed & 127, !!(control & 1), 518750 + (da & 127) * 6250);
}
static DEVICE_ATTR_RO(vcore_status);

'''
s=s[:a]+extra+s[a:]
s=s.replace('static struct attribute *y5_pmic_attrs[] = {','static struct attribute *y5_pmic_attrs[] = {\n &dev_attr_vcore_status.attr,')
s=s.replace('p->i2c = devm_platform_ioremap_resource(pdev, 2);','y5_register_vcore_readonly(p);\n\tp->i2c = devm_platform_ioremap_resource(pdev, 2);')
(w/'export/y5-pmic-v59.c').write_text(s,newline='\n')
(w/'compile_vcore_readonly.sh').write_text('''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
# Compile in an isolated directory, preserving the linked production source.
mkdir -p drivers/power/supply/mfw-bringup
cp /mnt/d/y5_linux_work/export/y5-pmic-v59.c drivers/power/supply/mfw-bringup/y5-pmic-v59.c
cp drivers/power/supply/y5-ntc.h drivers/power/supply/mfw-charger-state.h drivers/power/supply/mfw-hl-policy.h drivers/power/supply/mfw-bringup/
printf 'obj-y += y5-pmic-v59.o\\n' > drivers/power/supply/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/power/supply/mfw-bringup/y5-pmic-v59.o
''',newline='\n')
