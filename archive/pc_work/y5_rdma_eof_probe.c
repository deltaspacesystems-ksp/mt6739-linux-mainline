/* SPDX-License-Identifier: GPL-2.0-only
 * Bounded frame-end timing for the future DRM IRQ handler.
 * Only write: clear RDMA INT_STATUS using0, as mtk_disp_rdma_irq_handler.
 * No enable/mode/address/clock/power/DSI writes. Not safe with an RDMA driver.
 */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <glob.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
static uint64_t ns(void){struct timespec t;if(clock_gettime(CLOCK_MONOTONIC,&t))return 0;return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
int main(int argc,char **argv){
 if(argc!=2||strcmp(argv[1],"--sample-1s"))return 2;
 glob_t owners={0};int found=glob("/sys/bus/platform/devices/1400b000.*/driver",0,NULL,&owners);
 if(found!=GLOB_NOMATCH){globfree(&owners);fputs("RDMA owner or uncertain ownership; refusing\n",stderr);return 2;}globfree(&owners);
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *r=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x1400b000);
 volatile uint32_t *d=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14012000);
 if(r==MAP_FAILED||d==MAP_FAILED)return 2;
 if((d[0x14/4]&3)!=1 || !(r[0x10/4]&1))return 3;
 unsigned eof=0,underflows=0;uint64_t start=ns(),last=0,min=UINT64_MAX,max=0,total=0;
 /* Clear the pre-existing sticky snapshot once before timing new events. */
 r[1]=0;__asm__ volatile("dsb sy":::"memory");
 while(ns()-start<1000000000ULL){
  uint32_t status=r[1];
  if(status&(1U<<2)){
   uint64_t now=ns();if(last){uint64_t dt=now-last;if(dt<min)min=dt;if(dt>max)max=dt;total+=dt;}last=now;eof++;
   underflows+=!!(status&(1U<<4));r[1]=0;__asm__ volatile("dsb sy":::"memory");
  }
  struct timespec t={.tv_nsec=100000};nanosleep(&t,NULL);
 }
 printf("RDMA_EOF_SAMPLE duration_ns=%llu events=%u avg_period_us=%llu min_us=%llu max_us=%llu underflow_observations=%u\n",(unsigned long long)(ns()-start),eof,(unsigned long long)(eof>1?total/(eof-1)/1000:0),(unsigned long long)(eof>1?min/1000:0),(unsigned long long)(max/1000),underflows);
 munmap((void*)r,4096);munmap((void*)d,4096);close(fd);return eof>1?0:4;
}
