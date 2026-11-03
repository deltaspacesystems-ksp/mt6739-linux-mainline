/* SPDX-License-Identifier: GPL-2.0-only
 * MT6739 diagnostic reader. WACS2 offsets verified against stock 4.4.
 * No PMIC register writes: transport read-command/acknowledge only.
 * Fixed VLDO28 registers only; no ADC sampling or reconfiguration.
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
int main(void) {
 if(!access("/sys/bus/platform/devices/1000d000.pwrap/driver",F_OK)) { fputs("PWRAP owned: refusing\n",stderr); return 2; }
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 wrap=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x1000d000);
 if(wrap==MAP_FAILED){close(fd);return 2;}
 unsigned id;if(pmic_read(0xa,&id)||id!=0x5721){close(fd);return 3;}
 const unsigned regs[]={0x18bc,0x1b32,0x1b40,0x1b42,0x1c24,0x1c26};
 for(unsigned i=0;i<sizeof regs/sizeof regs[0];i++){unsigned value;if(pmic_read(regs[i],&value)){close(fd);return 3;}printf("VTOUCH[0x%04x]=0x%04x\n",regs[i],value);}
 munmap((void*)wrap,4096);close(fd);return 0;
}
