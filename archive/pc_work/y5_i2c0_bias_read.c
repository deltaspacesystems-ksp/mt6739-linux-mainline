/* SPDX-License-Identifier: GPL-2.0-only
 * Fixed NT50358A register0 READ. No sensor reset, supply or data writes.
 * Controller programming/index transmission only; exact restoration.
 */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
static volatile uint16_t *b;
static void wr(unsigned off,unsigned val){b[off/2]=val;__asm__ volatile("dsb sy":::"memory");}
static uint64_t now(void){struct timespec t;if(clock_gettime(CLOCK_MONOTONIC,&t))return 0;return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
int main(void){
 if(!access("/sys/bus/platform/devices/11007000.i2c/driver",F_OK))return 2;
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *infra=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10001000);
 if(infra==MAP_FAILED || (infra[0x90/4]&(1U<<11)))return 2;
 b=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x11007000);if(b==MAP_FAILED)return 2;
 if((b[0x24/2]&1)||((b[0x30/2]>>4)&15))return 3;
 const unsigned off[]={4,8,0x10,0x14,0x18,0x6c};unsigned saved[6];
 for(unsigned i=0;i<6;i++)saved[i]=b[off[i]/2];
 wr(8,saved[1]&~7);wr(0xc,0x1ff);wr(0x38,1);wr(0x10,0x3a);
 wr(4,0x3e<<1);wr(0x14,1);wr(0x6c,1);wr(0x18,2);wr(0,0);wr(0x24,1);
 unsigned status=0,val=0;uint64_t end=now()+20000000;
 do{status=b[0xc/2];if(status&7)break;}while(now()<end);
 unsigned count=(b[0x30/2]>>4)&15;int ok=(status&7)==1&&count==1;
 if(ok)val=b[0]&255;
 wr(0xc,status);wr(0x38,1);for(unsigned i=0;i<6;i++)wr(off[i],saved[i]);
 printf("LCD_BIAS_READ addr=0x3e reg=0 status=0x%04x count=%u valid=%d value=0x%02x\n",status,count,ok,val);
 munmap((void*)b,4096);munmap((void*)infra,4096);close(fd);return ok?0:4;
}
