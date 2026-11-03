/* SPDX-License-Identifier: GPL-2.0-only
 * Read-only active display OVL/SMI-LARB0/DSI snapshot. Does NOT map RDMA,
 * GPU, SPM or M4U, acknowledge IRQs, or alter any hardware registers.
 */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void){
 int fd=open("/dev/mem",O_RDONLY|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *o=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x1400a000);
 volatile uint32_t *l=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14003000);
 volatile uint32_t *d=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14012000);
 if(o==MAP_FAILED||l==MAP_FAILED||d==MAP_FAILED)return 2;
 printf("OVL_SRC_CON=%08x OVL_DATAPATH=%08x\n",o[0x2c/4],o[0x24/4]);
 for(unsigned i=0;i<8;i++)printf("LARB_NONSEC[%u]=%08x\n",i,l[(0x380+4*i)/4]);
 printf("LARB_LEGACY_F00=%08x LARB_LEGACY_FC0=%08x\n",l[0xf00/4],l[0xfc0/4]);
 const unsigned offsets[]={0x14,0x1c,0x20,0x24,0x28,0x2c,0x50,0x54,0x58};
 for(unsigned i=0;i<sizeof offsets/sizeof offsets[0];i++)printf("DSI[%02x]=%08x\n",offsets[i],d[offsets[i]/4]);
 munmap((void*)o,4096);munmap((void*)l,4096);munmap((void*)d,4096);close(fd);return 0;
}
