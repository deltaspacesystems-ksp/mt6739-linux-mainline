/* SPDX-License-Identifier: GPL-2.0-only
 * MT6739 diagnostic reader. WACS2 offsets verified against stock 4.4.
 * No reset, regulator or charger configuration. Mode 'v' writes only the
 * verified AUXADC request register, then clears it, as mainline does.
 * Run only while no kernel driver owns PWRAP. The command and acknowledge
 * writes below affect the wrapper transport, not PMIC register contents.
 */
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

static volatile uint32_t *wrap;
static uint64_t now_ns(void)
{
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t)) exit(2);
    return (uint64_t)t.tv_sec * 1000000000ULL + t.tv_nsec;
}
static uint32_t rd(unsigned off) { return wrap[off / 4]; }
static void wr(unsigned off, uint32_t v)
{
    wrap[off / 4] = v;
    __asm__ volatile("dsb sy" ::: "memory");
}
static int wait_fsm(unsigned wanted)
{
    uint64_t end = now_ns() + 20000000;
    do {
        uint32_t s = rd(0xc24);
        if (!(s & (1U << 21))) return -1;
        if (((s >> 16) & 7) == wanted) return 0;
    } while (now_ns() < end);
    return -1;
}
static int pmic_read(unsigned reg, unsigned *value)
{
    if (wait_fsm(0)) return -1;
    wr(0xc20, (reg >> 1) << 16); /* bit31=0: read only */
    if (wait_fsm(6)) return -1;
    *value = rd(0xc24) & 0xffff;
    wr(0xc28, 1);
    return wait_fsm(0);
}
static int adc_request(unsigned value)
{
    if ((value != 0 && value != 1 && value != 8) || wait_fsm(0)) return -1;
    wr(0xc20, (1U << 31) | ((0x110e >> 1) << 16) | value);
    return wait_fsm(0);
}
static int battery_voltage(int thermal)
{
    unsigned req, val = 0;
    int ret = 3;
    if (pmic_read(0x110e, &req) || req) {
        fputs("AUXADC request busy: leaving it unchanged\n", stderr);
        return 3;
    }
    if (adc_request(thermal ? 8 : 1)) {
        adc_request(0);
        return 3;
    }
    usleep(2000);
    uint64_t end = now_ns() + 100000000;
    do {
        if (pmic_read(thermal ? 0x108e : 0x1088, &val)) break;
        if (val & 0x8000) {
            printf("ADC raw=%u voltage_uV=%u\n", val & (thermal ? 0xfff : 0x7fff),
                   (unsigned)((uint64_t)(val & (thermal ? 0xfff : 0x7fff)) * (thermal ? 1800000 : 5400000) / (thermal ? 4096 : 32768)));
            ret = 0;
            break;
        }
        usleep(1000);
    } while (now_ns() < end);
    if (adc_request(0)) ret = 3;
    return ret;
}
int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    if (!access("/sys/bus/platform/devices/1000d000.pwrap/driver", F_OK)) {
        fputs("PWRAP has a kernel owner; refusing direct access\n", stderr);
        return 2;
    }
    static const struct { unsigned reg; const char *name; } regs[] = {
        {0xa88, "CHRDET"}, {0x8, "HWCID"}, {0xa, "SWCID"}, {0x24, "TOPSTATUS"},
        {0x1088, "AUXADC_ADC0_cached"}, {0x108a, "AUXADC_ADC1_cached"},
        {0x108e, "AUXADC_ADC3_cached"},
    };
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) { perror("/dev/mem"); return 2; }
    wrap = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x1000d000);
    close(fd);
    if (wrap == MAP_FAILED) { perror("mmap"); return 2; }
    if (argc == 2 && argv[1][0] == 'k' && !argv[1][1]) {
        fd = open("/dev/mem", O_RDONLY | O_SYNC);
        if (fd < 0) return 2;
        volatile uint16_t *kp = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0x10010000);
        close(fd);
        if (kp == MAP_FAILED) return 2;
        unsigned prev[5] = {0};
        uint64_t end = now_ns() + 180000000000ULL;
        do {
            unsigned val[5];
            int changed = 0;
            for (unsigned i = 0; i < 5; ++i) {
                val[i] = kp[(4 + i * 4) / 2];
                if (val[i] != prev[i]) changed = 1;
                prev[i] = val[i];
            }
            if (changed) printf("t_ms=%llu KPD_MEM=%04x %04x %04x %04x %04x\n",
                (unsigned long long)(now_ns()/1000000), val[0], val[1], val[2], val[3], val[4]);
            usleep(20000);
        } while (now_ns() < end);
        return 0;
    }
    printf("WACS2_RDATA=0x%08x INIT_DONE=%u FSM=%u\n", rd(0xc24),
           !!(rd(0xc24) & (1U << 21)), (rd(0xc24) >> 16) & 7);
    if (argc == 2 && argv[1][0] == 't' && !argv[1][1]) return battery_voltage(1);
    if (argc == 2 && argv[1][0] == 'v' && !argv[1][1])
        return battery_voltage(0);
    if (argc == 2 && argv[1][0] == 'w' && !argv[1][1]) {
        unsigned prev = ~0U;
        uint64_t end = now_ns() + 180000000000ULL;
        do {
            unsigned val;
            if (pmic_read(0x24, &val)) return 3;
            if (val != prev) {
                printf("t_ms=%llu TOPSTATUS=0x%04x power=%u resetkey=%u\n",
                       (unsigned long long)(now_ns() / 1000000), val,
                       !(val & 2), !(val & 8));
                prev = val;
            }
            usleep(20000);
        } while (now_ns() < end);
        return 0;
    }
    if (argc != 2 || argv[1][0] != 'r' || argv[1][1]) {
        puts("Status only. Argument 'r' reads the fixed PMIC register list.");
        return 0;
    }
    for (unsigned i = 0; i < sizeof(regs) / sizeof(regs[0]); ++i) {
        unsigned v;
        if (pmic_read(regs[i].reg, &v)) {
            fprintf(stderr, "PWRAP not ready or timeout at 0x%04x; status=0x%08x\n",
                    regs[i].reg, rd(0xc24));
            return 3;
        }
        printf("%s[0x%04x]=0x%04x\n", regs[i].name, regs[i].reg, v);
    }
    return 0;
}
