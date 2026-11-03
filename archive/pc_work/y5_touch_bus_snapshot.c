/* SPDX-License-Identifier: GPL-2.0-only */
/* Read-only GPIO/I2C0 snapshot. Never reads DATA_PORT or gated I2C MMIO. */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    if(access("/sys/bus/platform/devices/11007000.i2c/driver",F_OK)==0 ||
       access("/sys/bus/platform/devices/10005000.gpio/driver",F_OK)==0){
        fputs("Controller already owned; use its driver interfaces\n",stderr);return 1;
    }
    int fd=open("/dev/mem",O_RDONLY|O_SYNC);if(fd<0)return 2;
    volatile uint32_t *infra=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10001000);
    volatile uint32_t *gpio=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10005000);
    if(infra==MAP_FAILED||gpio==MAP_FAILED){close(fd);return 2;}
    uint32_t gate=infra[0x90/4];
    printf("INFRA_STA0=0x%08x I2C0_gate=%u I2C1_gate=%u\n",gate,!!(gate&(1U<<11)),!!(gate&(1U<<12)));
    for(unsigned pin=6;pin<=119;pin+=113){
        unsigned group=(pin/32)*0x10,bit=pin%32;
        printf("GPIO%u dir=%u out=%u in=%u\n",pin,
            !!(gpio[group/4]&(1U<<bit)),!!(gpio[(0x100+group)/4]&(1U<<bit)),!!(gpio[(0x200+group)/4]&(1U<<bit)));
    }
    if(gate&(1U<<11))puts("I2C0 gated; controller not accessed");
    else{
        volatile uint16_t *bus=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x11007000);
        if(bus==MAP_FAILED){close(fd);return 2;}
        const unsigned offsets[]={4,8,12,16,20,24,32,40,48,108,112};
        for(unsigned i=0;i<sizeof offsets/sizeof offsets[0];i++)printf("I2C0[0x%02x]=0x%04x\n",offsets[i],bus[offsets[i]/2]);
        munmap((void*)bus,4096);
    }
    munmap((void*)gpio,4096);munmap((void*)infra,4096);close(fd);return 0;
}
