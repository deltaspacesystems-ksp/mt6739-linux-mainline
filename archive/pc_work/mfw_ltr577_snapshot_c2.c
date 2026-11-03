/* SPDX-License-Identifier: GPL-2.0-only
 * Bounded, read-only LTR577 register snapshot, no scanning or force-address.
 * Evidence: stock ltr577_{i2c_probe,als_read,ps_read}, Huawei DURA-L21.
 * Does not enable measurements, emit calibrated lux or classify near/far.
 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int read_regs(int fd, uint8_t reg, uint8_t *buf, unsigned len)
{
    struct i2c_msg messages[] = {
        {.addr=0x53, .flags=0, .len=1, .buf=&reg},
        {.addr=0x53, .flags=I2C_M_RD, .len=(uint16_t)len, .buf=buf}
    };
    struct i2c_rdwr_ioctl_data transaction = {.msgs=messages, .nmsgs=2};
    int ret = ioctl(fd, I2C_RDWR, &transaction);
    if (ret == 2) return 0;
    if (ret >= 0) errno = EIO;
    perror("I2C read (no sample accepted)");
    return -1;
}

static unsigned le24(const uint8_t *b)
{
    return (unsigned)b[0] | ((unsigned)b[1]<<8) | ((unsigned)b[2]<<16);
}

int main(int argc, char **argv)
{
    if (argc == 2 && !strcmp(argv[1], "--help")) {
        puts("mfw-ltr577-snapshot --read-only\n"
             "Optional --read-only-stock-c2 uses measured c2 with stock raw map.\nFixed I2C-1 address 0x53; refuses a bound driver. Reads ID, "
             "control, gain/rate and raw channels only. No configuration "
             "writes; no clear-on-read status/IRQ registers. Samples may "
             "be stale when the sensor is disabled. No lux/near-far claims.");
        return 0;
    }
    int stock_c2 = argc == 2 && !strcmp(argv[1], "--read-only-stock-c2");
    if (argc != 2 || (strcmp(argv[1], "--read-only") && !stock_c2)) {
        fputs("Explicit --read-only required; see --help\n", stderr);
        return 2;
    }
    if (access("/sys/bus/i2c/devices/1-0053/driver", F_OK)==0) {
        fputs("Refusing: sensor has a kernel driver\n", stderr);
        return 1;
    }
    if (errno != ENOENT) {perror("driver ownership check"); return 1;}
    int fd=open("/dev/i2c-1", O_RDWR|O_CLOEXEC);
    if (fd < 0) {perror("I2C-1 unavailable; do not fall back to /dev/mem");return 1;}
    if (ioctl(fd,I2C_SLAVE,0x53)<0) {
        perror("Cannot select unclaimed LTR577 address (no FORCE)");
        close(fd); return 1;
    }
    unsigned long funcs=0;
    if (ioctl(fd,I2C_FUNCS,&funcs)<0 || !(funcs&I2C_FUNC_I2C)) {
        fputs("Adapter cannot perform combined I2C transfers\n",stderr);
        close(fd); return 1;
    }
    uint8_t id,ctrl,rate,gain,ps[2],als[3],clear[3];
    int ret=1;
    if (read_regs(fd,6,&id,1)<0) goto out;
    /* b1 has a special stock branch. Explicit c2 mode uses the same raw
     * register map present in stock non-b1 ALS/PS callers, not lux scaling.
     * c2 was measured on this board; exact marketing part remains unknown. */
    if ((!stock_c2 && id != 0xb1) || (stock_c2 && id != 0xc2)) {
        fprintf(stderr,"Unverified part ID 0x%02x; stopping before channel reads\n",id);
        goto out;
    }
    if (read_regs(fd,0,&ctrl,1)<0 || read_regs(fd,4,&rate,1)<0 ||
        read_regs(fd,5,&gain,1)<0 || read_regs(fd,8,ps,2)<0 ||
        read_regs(fd,10,clear,3)<0 || read_regs(fd,13,als,3)<0) goto out;
    printf("{\"part_id\":%u,\"main_ctrl\":%u,\"als_rate_raw\":%u,"
           "\"als_gain_raw\":%u,\"als_enabled\":%s,\"ps_enabled\":%s,"
           "\"als_raw\":%u,\"channel_0a_raw\":%u,\"proximity_raw\":%u,"
           "\"freshness_verified\":false,\"calibrated\":false}\n",
           id,ctrl,rate,gain,(ctrl&2)?"true":"false",(ctrl&1)?"true":"false",
           le24(als),le24(clear),(unsigned)ps[0]|((unsigned)(ps[1]&7)<<8));
    ret=0;
out:
    close(fd);
    return ret;
}
