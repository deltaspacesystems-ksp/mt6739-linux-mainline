// SPDX-License-Identifier: GPL-2.0-only
/* Native IIO raw light channels for the measured stock-compatible c2 part.
 * No lux conversion, proximity/IR enable, thresholds or calibration writes.
 * Initial integration uses the restricted MICROFW diagnostic I2C adapter.
 */
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/iio/iio.h>
#include <linux/delay.h>
#include <linux/mutex.h>
struct c2_data { struct i2c_client *client; struct mutex lock; };
static int c2_read(struct i2c_client *c, u8 reg, u8 *buf, u16 len)
{
 struct i2c_msg m[] = {
  { .addr = c->addr, .len = 1, .buf = &reg },
  { .addr = c->addr, .flags = I2C_M_RD, .len = len, .buf = buf },
 };
 int ret = i2c_transfer(c->adapter, m, 2);
 return ret == 2 ? 0 : ret < 0 ? ret : -EIO;
}
static int c2_control(struct i2c_client *c, u8 value)
{
 u8 buf[] = { 0, value };
 struct i2c_msg m = { .addr = c->addr, .len = 2, .buf = buf };
 int ret = i2c_transfer(c->adapter, &m, 1);
 return ret == 1 ? 0 : ret < 0 ? ret : -EIO;
}
static int c2_read_raw(struct iio_dev *indio, const struct iio_chan_spec *chan,
                       int *val, int *val2, long mask)
{
 struct c2_data *d = iio_priv(indio);
 u8 ctrl, bytes[3];
 int ret, restore;
 if (mask != IIO_CHAN_INFO_RAW) return -EINVAL;
 mutex_lock(&d->lock);
 ret = c2_read(d->client, 0, &ctrl, 1);
 if (ret) goto out;
 if (ctrl) { ret = -EBUSY; goto out; }
 ret = c2_control(d->client, 2);
 if (!ret) {
  msleep(500);
  ret = c2_read(d->client, chan->address, bytes, 3);
 }
 restore = c2_control(d->client, 0);
 if (!restore) restore = c2_read(d->client, 0, &ctrl, 1);
 if (!restore && ctrl) restore = -EIO;
 if (restore) { dev_err(&d->client->dev, "ALS restore failed: %d\n", restore); ret = restore; }
 if (!ret) { *val = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16); ret = IIO_VAL_INT; }
out:
 mutex_unlock(&d->lock);
 return ret;
}
static const struct iio_info c2_info = { .read_raw = c2_read_raw };
static const struct iio_chan_spec c2_channels[] = {
 { .type = IIO_LIGHT, .address = 13, .info_mask_separate = BIT(IIO_CHAN_INFO_RAW) },
 { .type = IIO_INTENSITY, .indexed = 1, .channel = 0, .extend_name = "channel0a", .address = 10,
   .info_mask_separate = BIT(IIO_CHAN_INFO_RAW) },
};
static int c2_probe(struct i2c_client *c)
{
 struct iio_dev *indio;
 struct c2_data *d;
 u8 id, ctrl, rate, gain;
 int ret;
 if (c->addr != 0x53) return -ENODEV;
 ret = c2_read(c, 6, &id, 1);
 if (ret) return ret;
 if (id != 0xc2) return -ENODEV;
 ret = c2_read(c, 0, &ctrl, 1);
 if (ret) return ret;
 if (ctrl) return -EBUSY;
 ret = c2_read(c, 4, &rate, 1);
 if (ret) return ret;
 ret = c2_read(c, 5, &gain, 1);
 if (ret) return ret;
 if (rate != 0x22 || gain != 1) return -EINVAL;
 indio = devm_iio_device_alloc(&c->dev, sizeof(*d));
 if (!indio) return -ENOMEM;
 d = iio_priv(indio); d->client = c; mutex_init(&d->lock);
 indio->name = "mfw-stock-c2-light";
 indio->info = &c2_info; indio->modes = INDIO_DIRECT_MODE;
 indio->channels = c2_channels; indio->num_channels = ARRAY_SIZE(c2_channels);
 return devm_iio_device_register(&c->dev, indio);
}
static const struct i2c_device_id c2_ids[] = { { "mfw_ltr_c2", 0 }, { } };
MODULE_DEVICE_TABLE(i2c, c2_ids);
static struct i2c_driver c2_driver = {
 .driver = { .name = "mfw-stock-c2-light" }, .probe = c2_probe, .id_table = c2_ids,
};
module_i2c_driver(c2_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MICROFW stock-c2 raw light IIO diagnostic driver");
