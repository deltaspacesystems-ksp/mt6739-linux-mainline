// SPDX-License-Identifier: GPL-2.0-only
/* DURA-L21 bootstrap: retain LK's PWRAP/regulator configuration.
 * This is deliberately not a complete MT6739 PWRAP/MFD driver.
 * Serialises WACS2 access, samples MT6357 VBAT, reports three buttons.
 * PMIC writes: AUXADC request only. HL7005D writes: TMR_RST and CE/TE only,
 * under the supported measured profile and the stock normal-temperature band.
 */
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/property.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/workqueue.h>
#include "y5-ntc.h"
#include "mfw-charger-state.h"
#include "mfw-hl-policy.h"

#define WACS_CMD 0xc20
#define WACS_RDATA 0xc24
#define WACS_VLDCLR 0xc28
#define INIT_DONE BIT(21)
#define TOPSTATUS 0x24
#define ADC_REQ 0x110e
#define ADC_BAT 0x1088

struct y5_pmic {
	struct device *dev;
	void __iomem *base;
	void __iomem *keypad;
	void __iomem *i2c;
	struct regmap *infra;
	bool charger_ready;
	struct delayed_work charger_work;
	struct power_supply *battery_psy, *usb_psy;
	unsigned long charge_started, charge_elapsed;
	bool charge_session, charge_profile_ok;
	unsigned int charge_blocks;
	int charge_temp, charge_last_error;
	u64 charge_kicks, charge_errors, charge_timer_faults;
	struct mutex lock;
	struct input_dev *input;
	struct delayed_work keys_work;
	unsigned int last_keys, pending_keys;
	bool key_valid, pending_valid;
};

/* Caller owns p->lock; the legacy I2C5 node is disabled in the DT.
 * Reads use an index + repeated START. Writes are limited to registers0/1;
 * wrappers below constrain their writable bits. LK bus timing is retained.
 */
static int y5_charger_transfer(struct y5_pmic *p, unsigned int reg,
		unsigned int *val, bool write)
{
	static const unsigned int offsets[] = {4, 8, 0x10, 0x14, 0x18, 0x6c};
	u16 saved[ARRAY_SIZE(offsets)], s;
	unsigned int i, gate;
	int ret;
	if (reg > 6 || (write && reg != 0 && reg != 1))
		return -EINVAL;
	if (write && (*val & ~0xffU))
		return -EINVAL;
	if (write && reg == 0 && (*val & ~0xc0U))
		return -EINVAL;
	if (!p->i2c || IS_ERR_OR_NULL(p->infra))
		return -ENODEV;
	ret = regmap_read(p->infra, 0xac, &gate);
	if (ret || (gate & BIT(18)))
		return ret ? ret : -EHOSTDOWN;
	if ((readw(p->i2c + 0x24) & 1) || ((readw(p->i2c + 0x30) >> 4) & 15))
		return -EBUSY;
	for (i = 0; i < ARRAY_SIZE(offsets); i++)
		saved[i] = readw(p->i2c + offsets[i]);
	writew(saved[1] & ~7, p->i2c + 8);
	writew(0x1ff, p->i2c + 0xc);
	writew(1, p->i2c + 0x38);
	writew(write ? 0x28 : 0x3a, p->i2c + 0x10);
	writew(0x6a << 1, p->i2c + 4);
	writew(write ? 2 : 1, p->i2c + 0x14);
	writew(write ? 0 : 1, p->i2c + 0x6c);
	writew(write ? 1 : 2, p->i2c + 0x18);
	writew(reg, p->i2c);
	if (write)
		writew(*val, p->i2c);
	writew(1, p->i2c + 0x24);
	ret = readw_poll_timeout(p->i2c + 0xc, s, s & 7, 50, 20000);
	if (!ret && ((s & 7) != 1))
		ret = -EIO;
	if (!ret && (((readw(p->i2c + 0x30) >> 4) & 15) != (write ? 0 : 1)))
		ret = -EIO;
	if (!ret && !write)
		*val = readw(p->i2c) & 255;
	writew(s, p->i2c + 0xc);
	for (i = 0; i < ARRAY_SIZE(offsets); i++)
		writew(saved[i], p->i2c + offsets[i]);
	return ret;
}

static int y5_charger_read(struct y5_pmic *p, unsigned int reg, unsigned int *val)
{
	return y5_charger_transfer(p, reg, val, false);
}

static int y5_charger_control_write(struct y5_pmic *p, unsigned int old, unsigned int val)
{
	if ((old ^ val) & ~12U)
		return -EINVAL; /* Only CE and TE; never voltage/current/boost settings. */
	return y5_charger_transfer(p, 1, &val, true);
}

static int y5_wait(struct y5_pmic *p, unsigned int fsm)
{
	u32 s;
	int ret = readl_poll_timeout(p->base + WACS_RDATA, s,
		!(s & INIT_DONE) || ((s >> 16) & 7) == fsm, 5, 20000);
	return ret ? ret : ((s & INIT_DONE) ? 0 : -ENODEV);
}

/* Caller owns p->lock. Never consumes stale WFVLDCLR data from another owner. */
static int y5_read(struct y5_pmic *p, unsigned int reg, unsigned int *val)
{
	int ret = y5_wait(p, 0);
	if (ret)
		return ret;
	writel((reg >> 1) << 16, p->base + WACS_CMD);
	ret = y5_wait(p, 6);
	if (ret)
		return ret;
	*val = readl(p->base + WACS_RDATA) & 0xffff;
	writel(1, p->base + WACS_VLDCLR);
	return y5_wait(p, 0);
}

static int y5_adc_request(struct y5_pmic *p, unsigned int val)
{
	int ret = y5_wait(p, 0);
	if (ret)
		return ret;
	if (val != 0 && val != 1 && val != BIT(3))
		return -EINVAL;
	writel(BIT(31) | ((ADC_REQ >> 1) << 16) | val, p->base + WACS_CMD);
	return y5_wait(p, 0);
}

static int y5_sample(struct y5_pmic *p, bool thermal, unsigned int *raw)
{
	unsigned int req, adc;
	unsigned long deadline;
	int ret, stop_ret;

	mutex_lock(&p->lock);
	ret = y5_read(p, ADC_REQ, &req);
	if (ret || req) {
		if (!ret)
			ret = -EBUSY;
		goto out;
	}
	ret = y5_adc_request(p, thermal ? BIT(3) : 1);
	if (ret)
		goto stop;
	usleep_range(2000, 2500);
	deadline = jiffies + msecs_to_jiffies(100);
	do {
		ret = y5_read(p, thermal ? 0x108e : ADC_BAT, &adc);
		if (ret)
			break;
		if (adc & BIT(15)) {
			*raw = adc & (thermal ? 0xfff : 0x7fff);
			goto stop;
		}
		usleep_range(1000, 1500);
	} while (time_before(jiffies, deadline));
	if (!ret)
		ret = -ETIMEDOUT;
stop:
	stop_ret = y5_adc_request(p, 0);
	if (!ret)
		ret = stop_ret;
out:
	mutex_unlock(&p->lock);
	return ret;
}

struct y5_charger_snapshot {
	unsigned int status, control, oreg, ibat, special, safety, online;
};

static int y5_charger_snapshot(struct y5_pmic *p, struct y5_charger_snapshot *s)
{
	unsigned int det;
	int ret;
	if (!p->charger_ready)
		return -ENODEV;
	mutex_lock(&p->lock);
	ret = y5_charger_read(p, 0, &s->status);
	if (!ret)
		ret = y5_charger_read(p, 1, &s->control);
	if (!ret)
		ret = y5_charger_read(p, 5, &s->special);
	if (!ret)
		ret = y5_charger_read(p, 2, &s->oreg);
	if (!ret)
		ret = y5_charger_read(p, 4, &s->ibat);
	if (!ret)
		ret = y5_charger_read(p, 6, &s->safety);
	if (!ret)
		ret = y5_read(p, 0xa88, &det);
	if (!ret)
		s->online = !!(det & BIT(4));
	mutex_unlock(&p->lock);
	return ret;
}

static int y5_battery_status(struct y5_pmic *p, int *status)
{
	struct y5_charger_snapshot s;
	static const int states[] = {
		POWER_SUPPLY_STATUS_DISCHARGING, POWER_SUPPLY_STATUS_NOT_CHARGING,
		POWER_SUPPLY_STATUS_CHARGING, POWER_SUPPLY_STATUS_FULL,
	};
	int ret = y5_charger_snapshot(p, &s);
	if (!ret)
		*status = states[mfw_charge_state(s.status, s.control, s.special, s.online)];
	return ret;
}

static enum power_supply_property y5_battery_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_TEMP,
};

static int y5_get_property(struct power_supply *psy,
			   enum power_supply_property prop,
			   union power_supply_propval *val)
{
	struct y5_pmic *p = power_supply_get_drvdata(psy);
	unsigned int raw;
	int ret;
	if (prop == POWER_SUPPLY_PROP_STATUS) {
		if (!p->charger_ready) {
			val->intval = POWER_SUPPLY_STATUS_UNKNOWN;
			return 0;
		}
		return y5_battery_status(p, &val->intval);
	}
	if (prop == POWER_SUPPLY_PROP_VOLTAGE_NOW || prop == POWER_SUPPLY_PROP_TEMP) {
		ret = y5_sample(p, prop == POWER_SUPPLY_PROP_TEMP, &raw);
		if (ret)
			return ret;
		if (prop == POWER_SUPPLY_PROP_TEMP)
			return y5_ntc_deci_c(raw, &val->intval) ? -ENODATA : 0;
		val->intval = div_u64((u64)raw * 5400000, 32768);
		return 0;
	}
	return -EINVAL;
}

static const struct power_supply_desc y5_battery_desc = {
	.name = "microfw-battery",
	.type = POWER_SUPPLY_TYPE_BATTERY,
	.properties = y5_battery_props,
	.num_properties = ARRAY_SIZE(y5_battery_props),
	.get_property = y5_get_property,
};

static enum power_supply_property y5_usb_props[] = {
	POWER_SUPPLY_PROP_ONLINE, POWER_SUPPLY_PROP_STATUS, POWER_SUPPLY_PROP_MODEL_NAME,
};

static int y5_usb_get_property(struct power_supply *psy, enum power_supply_property prop,
			     union power_supply_propval *val)
{
	struct y5_pmic *p = power_supply_get_drvdata(psy);
	struct y5_charger_snapshot s;
	int ret;
	if (prop == POWER_SUPPLY_PROP_MODEL_NAME) {
		val->strval = "HL7005D";
		return 0;
	}
	if (prop == POWER_SUPPLY_PROP_STATUS)
		return y5_battery_status(p, &val->intval);
	if (prop != POWER_SUPPLY_PROP_ONLINE)
		return -EINVAL;
	ret = y5_charger_snapshot(p, &s);
	if (!ret)
		val->intval = s.online;
	return ret;
}

static const struct power_supply_desc y5_usb_desc = {
	.name = "microfw-usb",
	.type = POWER_SUPPLY_TYPE_USB,
	.properties = y5_usb_props,
	.num_properties = ARRAY_SIZE(y5_usb_props),
	.get_property = y5_usb_get_property,
};

static ssize_t charger_registers_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct y5_charger_snapshot s;
	int ret = y5_charger_snapshot(dev_get_drvdata(dev), &s);
	if (ret)
		return ret;
	return sysfs_emit(buf, "id=0x41 status=0x%02x control=0x%02x special=0x%02x oreg=0x%02x ibat=0x%02x safety=0x%02x fault=%u online=%u\n",
			s.status, s.control, s.special, s.oreg, s.ibat, s.safety,
			s.status & 7, s.online);
}
static DEVICE_ATTR_RO(charger_registers);

static ssize_t charger_service_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct y5_pmic *p = dev_get_drvdata(dev);
	ssize_t n;
	mutex_lock(&p->lock);
	n = sysfs_emit(buf, "kicks=%llu errors=%llu timer_faults_seen=%llu last_error=%d profile_ok=%u blocks=%u temp=%d elapsed_s=%lu\n",
			p->charge_kicks, p->charge_errors, p->charge_timer_faults, p->charge_last_error,
			p->charge_profile_ok, p->charge_blocks, p->charge_temp, p->charge_elapsed);
	mutex_unlock(&p->lock);
	return n;
}
static DEVICE_ATTR_RO(charger_service);
static struct attribute *y5_pmic_attrs[] = {
	&dev_attr_charger_registers.attr, &dev_attr_charger_service.attr, NULL
};
ATTRIBUTE_GROUPS(y5_pmic);

static void y5_charger_service(struct work_struct *work)
{
	struct y5_pmic *p = container_of(to_delayed_work(work), struct y5_pmic, charger_work);
	struct y5_charger_snapshot s;
	unsigned int raw, control, wanted, timer, verify, old_blocks;
	int temp = -999, temp_ret, ret;
	bool changed = false;

	/* This helper serialises its ADC transaction separately. */
	temp_ret = y5_sample(p, true, &raw);
	if (!temp_ret && y5_ntc_deci_c(raw, &temp))
		temp_ret = -ENODATA;
	ret = y5_charger_snapshot(p, &s);
	mutex_lock(&p->lock);
	if (ret)
		goto error;
	p->charge_temp = temp;
	if (((s.status >> 4) & 3) == 3 && (s.status & 7) == 6) {
		p->charge_timer_faults++;
		changed = true;
	}
	if (!s.online || (s.status & BIT(3))) {
		changed = p->charge_session;
		p->charge_session = false;
		p->charge_elapsed = 0;
		p->charge_blocks = 0;
		p->charge_profile_ok = false;
		p->charge_last_error = 0;
		goto out;
	}
	if (!p->charge_session) {
		p->charge_session = true;
		p->charge_started = jiffies;
		changed = true;
	}
	p->charge_elapsed = (jiffies - p->charge_started) / HZ;
	p->charge_profile_ok = mfw_hl_profile_supported(s.control, s.oreg,
						      s.ibat, s.special, s.safety);
	if (!p->charge_profile_ok) {
		ret = -ERANGE;
		goto error; /* Never rewrite unknown voltage/current settings. */
	}
	old_blocks = p->charge_blocks;
	p->charge_blocks = mfw_hl_blocks(!temp_ret, temp, p->charge_elapsed);
	changed |= old_blocks != p->charge_blocks;

	/* Re-read live control before changing CE/TE to avoid restoring stale bits. */
	ret = y5_charger_read(p, 1, &control);
	if (ret)
		goto error;
	if (!mfw_hl_profile_supported(control, s.oreg, s.ibat, s.special, s.safety)) {
		ret = -ERANGE;
		goto error;
	}
	wanted = mfw_hl_control(control, p->charge_blocks);
	if (wanted != control) {
		ret = y5_charger_control_write(p, control, wanted);
		if (ret)
			goto error;
		changed = true;
	}
	ret = y5_charger_read(p, 0, &verify);
	if (ret)
		goto error;
	timer = mfw_hl_timer_value(verify);
	ret = y5_charger_transfer(p, 0, &timer, true);
	if (ret)
		goto error;
	ret = y5_charger_read(p, 1, &verify);
	if (ret)
		goto error;
	if (verify != wanted) {
		ret = -EIO;
		goto error;
	}
	p->charge_kicks++;
	if (p->charge_kicks == 1)
		dev_info(p->dev, "HL7005D: watchdog service 8s, 4.20V profile, CE/TE supervision\n");
	p->charge_last_error = temp_ret;
	if (temp_ret)
		p->charge_errors++;
	goto out;
error:
	p->charge_errors++;
	if (p->charge_last_error != ret)
		dev_warn(p->dev, "HL7005D service paused: %d\n", ret);
	p->charge_last_error = ret;
out:
	mutex_unlock(&p->lock);
	if (changed) {
		power_supply_changed(p->battery_psy);
		power_supply_changed(p->usb_psy);
	}
	schedule_delayed_work(&p->charger_work, msecs_to_jiffies(8000));
}

static void y5_keys_poll(struct work_struct *work)
{
	struct y5_pmic *p = container_of(to_delayed_work(work), struct y5_pmic, keys_work);
	unsigned int s, keys;
	int ret;

	mutex_lock(&p->lock);
	ret = y5_read(p, TOPSTATUS, &s);
	mutex_unlock(&p->lock);
	if (ret) {
		/* Require two consecutive successful observations after an error. */
		p->pending_valid = false;
		dev_warn_ratelimited(p->dev, "key read failed: %d\n", ret);
		goto again;
	}
	/* Two controlled volume-down presses confirmed KPD_MEM0 bit 0.
	 * LK's matrix setup mirrors it every nine bits; report it only once.
	 */
	keys = (s & 0xa) | ((readw(p->keypad + 4) & 1) ? BIT(4) : 0);
	if (p->pending_valid && p->pending_keys == keys &&
	    (!p->key_valid || p->last_keys != keys)) {
		input_report_key(p->input, KEY_POWER, !(keys & BIT(1)));
		input_report_key(p->input, KEY_VOLUMEUP, !(keys & BIT(3)));
		input_report_key(p->input, KEY_VOLUMEDOWN, !(keys & BIT(4)));
		input_sync(p->input);
		/* Physical identity confirmed by the owner's controlled button test. */
		dev_info(p->dev, "keys: power=%u volume_up=%u volume_down=%u\n",
			 !(keys & BIT(1)), !(keys & BIT(3)), !(keys & BIT(4)));
		p->last_keys = keys;
		p->key_valid = true;
	}
	p->pending_keys = keys;
	p->pending_valid = true;
again:
	schedule_delayed_work(&p->keys_work, msecs_to_jiffies(30));
}

static void y5_stop_poll(void *data)
{
	struct y5_pmic *p = data;
	cancel_delayed_work_sync(&p->keys_work);
	cancel_delayed_work_sync(&p->charger_work);
}

static int y5_pmic_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct power_supply_config cfg = {};
	struct power_supply *psy;
	struct y5_pmic *p;
	unsigned int id;
	int ret;

	p = devm_kzalloc(dev, sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;
	p->dev = dev;
	p->charge_temp = -999;
	p->charge_last_error = -EAGAIN;
	platform_set_drvdata(pdev, p);
	mutex_init(&p->lock);
	p->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(p->base))
		return PTR_ERR(p->base);
	p->keypad = devm_platform_ioremap_resource(pdev, 1);
	if (IS_ERR(p->keypad))
		return PTR_ERR(p->keypad);
	ret = y5_read(p, 0xa, &id);
	if (ret)
		return dev_err_probe(dev, ret, "LK PWRAP unavailable; no reinitialisation\n");
	if ((id >> 8) != 0x57)
		return dev_err_probe(dev, -ENODEV, "unexpected PMIC ID 0x%04x\n", id);
	p->i2c = devm_platform_ioremap_resource(pdev, 2);
	if (IS_ERR(p->i2c))
		p->i2c = NULL;
	p->infra = syscon_regmap_lookup_by_phandle(dev->of_node, "microfw,infracfg");
	ret = y5_charger_read(p, 3, &id);
	p->charger_ready = !ret && id == 0x41;
	if (!p->charger_ready)
		dev_warn(dev, "HL7005D monitor unavailable (ret=%d); PMIC/keys remain active\n", ret);

	p->input = devm_input_allocate_device(dev);
	if (!p->input)
		return -ENOMEM;
	p->input->name = "microfw MT6357 power and volume keys";
	p->input->phys = "y5-pmic/input0";
	p->input->id.bustype = BUS_HOST;
	input_set_capability(p->input, EV_KEY, KEY_POWER);
	input_set_capability(p->input, EV_KEY, KEY_VOLUMEUP);
	input_set_capability(p->input, EV_KEY, KEY_VOLUMEDOWN);
	ret = input_register_device(p->input);
	if (ret)
		return ret;
	cfg.drv_data = p;
	cfg.fwnode = dev_fwnode(dev);
	psy = devm_power_supply_register(dev, &y5_battery_desc, &cfg);
	if (IS_ERR(psy))
		return PTR_ERR(psy);
	p->battery_psy = psy;
	if (p->charger_ready) {
		psy = devm_power_supply_register(dev, &y5_usb_desc, &cfg);
		if (IS_ERR(psy))
			return PTR_ERR(psy);
		p->usb_psy = psy;
		dev_info(dev, "HL7005D ID=0x41: charge status, USB detection and guarded service\n");
	}
	INIT_DELAYED_WORK(&p->keys_work, y5_keys_poll);
	INIT_DELAYED_WORK(&p->charger_work, y5_charger_service);
	ret = devm_add_action_or_reset(dev, y5_stop_poll, p);
	if (ret)
		return ret;
	schedule_delayed_work(&p->keys_work, msecs_to_jiffies(100));
	if (p->charger_ready)
		schedule_delayed_work(&p->charger_work, msecs_to_jiffies(500));
	dev_info(dev, "MT6357: VBAT + NTC + three buttons\n");
	return 0;
}

static const struct of_device_id y5_pmic_of_match[] = {
	{ .compatible = "microfw,dura-l21-pmic-monitor" },
	{}
};
MODULE_DEVICE_TABLE(of, y5_pmic_of_match);

static void y5_pmic_shutdown(struct platform_device *pdev)
{
	y5_stop_poll(platform_get_drvdata(pdev));
}

static struct platform_driver y5_pmic_driver = {
	.probe = y5_pmic_probe,
	.shutdown = y5_pmic_shutdown,
	.driver = {
		.name = "y5-pmic-monitor",
		.of_match_table = y5_pmic_of_match,
		.dev_groups = y5_pmic_groups,
	},
};
module_platform_driver(y5_pmic_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DURA-L21 MT6357 and guarded HL7005D bootstrap");
