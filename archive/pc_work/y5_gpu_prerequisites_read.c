/* SPDX-License-Identifier: GPL-2.0-only
 * GPU prerequisites from always-on blocks only. NEVER maps GPU/MFG_CFG.
 * Register offsets cross-checked against the pinned MT6739 vendor source.
 */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void){int fd=open("/dev/mem",O_RDONLY|O_SYNC);if(fd<0)return 2;
 volatile uint32_t*t=mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x10000000);
 volatile uint32_t*s=mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x10006000);
 volatile uint32_t*i=mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x10001000);
 volatile uint32_t*p=mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x1000c000);
 if(t==MAP_FAILED||s==MAP_FAILED||i==MAP_FAILED||p==MAP_FAILED)return 2;
 uint32_t cfg=t[0x50/4];printf("MFG_TOP_CFG1=%08x parent_index=%u gate_disabled=%u\n",cfg,(cfg>>24)&3,(cfg>>31)&1);
 printf("SPM_STATUS=%08x,%08x MFG_CTRL=%08x CORE_CTRL=%08x\n",s[0x180/4],s[0x184/4],s[0x338/4],s[0x33c/4]);
 printf("INFRA_SI0_CTL=%08x PROTECT_ENABLE=%08x PROTECT_STA1=%08x\n",i[0x200/4],i[0x220/4],i[0x228/4]);
 printf("MFGPLL_CON0=%08x CON1=%08x CON2=%08x PWR=%08x\n",p[0x240/4],p[0x244/4],p[0x248/4],p[0x24c/4]);
 munmap((void*)t,4096);munmap((void*)s,4096);munmap((void*)i,4096);munmap((void*)p,4096);close(fd);return 0;}
