/* SPDX-License-Identifier: GPL-2.0-only
 * Always-on SPM status only. NEVER maps/reads GPU core MMIO, powers it up,
 * changes clocks, bus protection, isolation or regulators.
 * MFG bit4/control338 verified against stock spm_mtcmos_ctrl_mfg.
 */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void){
 int fd=open("/dev/mem",O_RDONLY|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *s=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10006000);if(s==MAP_FAILED)return 2;
 unsigned a=s[0x180/4],b=s[0x184/4];
 printf("SPM_PWR_STATUS=%08x SPM_PWR_STATUS_2ND=%08x MFG_PWR_CON=%08x MFG_CORE0_PWR_CON=%08x\n",a,b,s[0x338/4],s[0x33c/4]);
 printf("MFG_STATUS_BIT4=%u,%u GPU_MMIO_NOT_ACCESSED=1\n",!!(a&(1U<<4)),!!(b&(1U<<4)));
 munmap((void*)s,4096);close(fd);return 0;
}
