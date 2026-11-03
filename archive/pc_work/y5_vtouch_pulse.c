/* SPDX-License-Identifier: GPL-2.0-only
 * MT6739 bounded touch power diagnostic. WACS2 offsets verified against stock 4.4.
 * PMIC data writes are restricted to VLDO28 enable; no voltage writes.
 * Temporary VLDO28 enable; previous enable state must be restored.
 * Run only while no kernel driver owns PWRAP. The command and acknowledge
 * writes below affect the wrapper transport, not PMIC register contents.
 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

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

/* Only the verified VLDO28 enable bit; voltage selector is never written. */
static int vtouch_enable(unsigned value)
{
 if(value>1 || wait_fsm(0))return -1;
 wr(0xc20, (1U<<31) | ((0x1b32U>>1)<<16) | value);
 return wait_fsm(0);
}
int main(int argc,char **argv)
{
 if(argc!=2 || strcmp(argv[1],"--pulse-vtouch"))return 2;
 if(!access("/sys/bus/platform/devices/1000d000.pwrap/driver",F_OK))return 2;
 if(!access("/sys/bus/platform/devices/10005000.gpio/driver",F_OK))return 2;
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *gpio=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10005000);
 wrap=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x1000d000);
 if(gpio==MAP_FAILED || wrap==MAP_FAILED)return 2;
 /* RST119 must already be GPIO output LOW. Never change its mux or latch. */
 if((gpio[0x3e0/4]>>28)!=0 || !(gpio[0x30/4]&(1U<<23)) ||
    (gpio[0x130/4]&(1U<<23))){puts("RESET not held low; refusing");return 3;}
 unsigned id,vsel,before,enabled=0,after=0;
 if(pmic_read(0xa,&id)||id!=0x5721 || pmic_read(0x1c24,&vsel) ||
    (vsel&0x300)!=0x100 || pmic_read(0x1b32,&before) || before!=0){
   puts("Unexpected PMIC/voltage/enable; no data writes");return 3;
 }
 printf("VTOUCH before=0x%04x selector=0x%04x (2.8V), RST held LOW\n",before,vsel);
 /* Mark the write as attempted even on timeout; cleanup must restore OFF. */
 int ret=vtouch_enable(1);
 if(!ret){struct timespec t={.tv_nsec=150000000};nanosleep(&t,NULL);
   ret=pmic_read(0x1b32,&enabled);if(!ret && enabled!=1)ret=-1;}
 int restore=vtouch_enable(before);
 if(!restore)restore=pmic_read(0x1b32,&after);
 if(!restore && after!=before)restore=-1;
 printf("VTOUCH enabled=0x%04x restored=0x%04x result=%d restore=%d\n",enabled,after,ret,restore);
 munmap((void*)gpio,4096);munmap((void*)wrap,4096);close(fd);
 return restore?5:ret?4:0;
}
