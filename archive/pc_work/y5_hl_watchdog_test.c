/* SPDX-License-Identifier: GPL-2.0-only */
/* Bounded 80-second lab test on the unbound v52 charger.
 * Register0 TMR_RST strobes only, preserving EN_STAT and all profile settings.
 * No voltage/current/safety-register writes; fixed measured profile guard.
 */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
static volatile uint16_t *bus;
static unsigned rd(unsigned o) { return bus[o/2]; }
static void wr(unsigned o, unsigned v)
{
    bus[o/2] = v;
    __asm__ volatile("dsb sy" ::: "memory");
}
static uint64_t now(void)
{
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t)) return 0;
    return (uint64_t)t.tv_sec*1000000000ULL + t.tv_nsec;
}
static int read_reg(unsigned reg, unsigned *value)
{
    wr(0x0c, 0x1ff); /* W1C old controller flags */
    wr(0x38, 1);     /* FIFO address clear */
    wr(0x10, 0x3a);  /* ACKERR_DET, CLK_EXT, DIR_CHANGE, repeated START, no DMA */
    wr(4, 0x6a << 1);
    wr(0x14, 1);     /* exactly one index byte, never charger data */
    wr(0x6c, 1);     /* one read byte in auxiliary-length mode */
    wr(0x18, 2);
    wr(0, reg);
    wr(0x24, 1);
    uint64_t end = now() + 20000000;
    unsigned status;
    do {
        status = rd(0xc);
        if (status & 7) break;
        usleep(50);
    } while (now() < end);
    printf("reg=0x%02x intr=0x%04x fifo=0x%04x start=0x%04x\n",
           reg, status, rd(0x30), rd(0x24));
    if ((status & 7) != 1 || ((rd(0x30) >> 4) & 0xf) != 1) return -1;
    *value = rd(0) & 255;
    wr(0xc, status);
    return 0;
}
static int timer_reset(unsigned value)
{
    wr(0xc,0x1ff); wr(0x38,1); wr(0x10,0x28);
    wr(4,0x6a<<1); wr(0x14,2); wr(0x6c,0); wr(0x18,1);
    wr(0,0); wr(0,value); wr(0x24,1);
    uint64_t end=now()+20000000;
    unsigned s;
    do { s=rd(0xc); if (s&7) break; usleep(50); } while (now()<end);
    if ((s&7)!=1 || ((rd(0x30)>>4)&15)) return -1;
    wr(0xc,s);
    return 0;
}
int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    if (!access("/sys/bus/platform/devices/1000d000.pwrap/of_node/microfw,charger-monitor", F_OK) && !access("/sys/bus/platform/devices/1000d000.pwrap/driver", F_OK)) return 2;
    if (!access("/sys/bus/platform/devices/11016000.i2c/driver", F_OK)) return 2;
    int fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd < 0) return 2;
    volatile uint32_t *infra = mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x10001000);
    if (infra == MAP_FAILED || (infra[0xac/4] & (1U<<18))) return 2;
    bus = mmap(0,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x11016000);
    close(fd);
    if (bus == MAP_FAILED || (rd(0x24) & 1) || ((rd(0x30)>>4)&15)) return 2;
    const unsigned offsets[] = {4,8,0x10,0x14,0x18,0x6c};
    unsigned saved[6];
    for (unsigned i=0;i<6;i++) saved[i]=rd(offsets[i]);
    wr(8, saved[1] & ~7U); /* polling only, no new AP interrupt */
    const unsigned regs[] = {0,1,2,3,4,5,6};
    unsigned profile[7];
    int result=0;
    for (unsigned i=0;i<7;i++) {
        if (read_reg(regs[i], &profile[i])) { result=3; goto restore; }
        printf("PROFILE[%u]=0x%02x\n", i, profile[i]);
    }
    /* Only the measured low-input/default profile is permitted in this test. */
    if (profile[3]!=0x41 || profile[1]!=0x70 || profile[2]!=0x8e ||
        (profile[4]&0x7f)!=1 || (profile[5]&0xef)!=0x24 || profile[6]!=0x7b) {
        puts("Unexpected profile: no charger data write performed");
        result=3; goto restore;
    }
    for (unsigned cycle=0;cycle<10;cycle++) {
        unsigned state;
        if (read_reg(0,&state) || (state&8) || timer_reset((state&0x40)|0x80)) {
            result=3; break;
        }
        printf("KICK=%u value=0x%02x\n",cycle+1,(state&0x40)|0x80);
        for (unsigned reg=1;reg<7;reg++) {
            unsigned value;
            if (read_reg(reg,&value)) { result=3; break; }
            unsigned mask=reg==4?0x7f:reg==5?0xef:0xff;
            if ((value&mask)!=(profile[reg]&mask)) {
                printf("PROFILE_CHANGED reg=%u value=0x%02x\n",reg,value);
                result=3; break;
            }
        }
        if (result || read_reg(0,&state)) { result=3; break; }
        printf("STATUS_AFTER_KICK=0x%02x fault=%u\n",state,state&7);
        if (((state>>4)&3)==3) { result=3; break; }
        usleep(8000000);
    }
    printf("TEST_DONE result=%d\n",result);
restore:
    for (unsigned i=0;i<6;i++) wr(offsets[i],saved[i]);
    return result;
}
