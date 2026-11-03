/* SPDX-License-Identifier: GPL-2.0-only
 * Read-only active display registers. Never accesses GPU or clear-on-read
 * FIFO/data ports; no IRQ acknowledgements, power or clock changes.
 * Offsets cross-checked with Linux7.3 mtk_disp_rdma.c/mtk_dsi.c.
 */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
int main(void){
 int fd=open("/dev/mem",O_RDONLY|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *r=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x1400b000);
 volatile uint32_t *d=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14012000);
 volatile uint32_t *m=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14015000);
 volatile uint32_t *o=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x1400a000);
 volatile uint32_t *mt=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14001000);
 if(r==MAP_FAILED||d==MAP_FAILED||m==MAP_FAILED||o==MAP_FAILED||mt==MAP_FAILED)return 2;
 const unsigned ro[]={0,4,0x10,0x14,0x18,0x24,0x2c,0xf00};
 const unsigned ds[]={0,4,8,0xc,0x10,0x14,0x18,0x1c,0x20};
 const unsigned ov[]={4,8,0xc,0x20,0x2c,0x30,0x34,0x38,0x3c,0x40,0x44,0xc0,0xf40};
 for(unsigned i=0;i<sizeof ro/sizeof ro[0];i++)printf("RDMA[%03x]=%08x\n",ro[i],r[ro[i]/4]);
 for(unsigned i=0;i<sizeof ds/sizeof ds[0];i++)printf("DSI[%03x]=%08x\n",ds[i],d[ds[i]/4]);
 for(unsigned i=0;i<sizeof ov/sizeof ov[0];i++)printf("OVL[%03x]=%08x\n",ov[i],o[ov[i]/4]);
 for(unsigned layer=0;layer<4;layer++)printf("OVL_LAYER%u enabled=%u con=%08x size=%08x offset=%08x pitch=%u addr=%08x\n",layer,!!(o[0x2c/4]&(1U<<layer)),o[(0x30+layer*0x20)/4],o[(0x38+layer*0x20)/4],o[(0x3c+layer*0x20)/4],o[(0x44+layer*0x20)/4]&65535,o[(0xf40+layer*0x20)/4]);
 printf("LEGACY_14015020=%08x TRUE_MUTEX_EN=%08x TRUE_MUTEX_RST=%08x TRUE_MUTEX_MOD=%08x TRUE_MUTEX_SOF=%08x\n",m[0x20/4],mt[0x20/4],mt[0x28/4],mt[0x2c/4],mt[0x30/4]);
 unsigned busy=0,rdma_or=0,dsi_or=0,changes=0,last=d[3];
 for(unsigned i=0;i<1000;i++){uint32_t s=d[3];busy+=!!(s&(1U<<31));changes+=s!=last;last=s;dsi_or|=s;rdma_or|=r[1];struct timespec t={.tv_nsec=1000000};nanosleep(&t,NULL);}
 printf("SAMPLES=1000 DSI_BUSY_SAMPLES=%u DSI_STATUS_CHANGES=%u DSI_OR=%08x RDMA_OR=%08x\n",busy,changes,dsi_or,rdma_or);
 munmap((void*)r,4096);munmap((void*)d,4096);munmap((void*)m,4096);munmap((void*)o,4096);munmap((void*)mt,4096);close(fd);return 0;
}
