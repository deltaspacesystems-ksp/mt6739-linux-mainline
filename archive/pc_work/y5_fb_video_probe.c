/* SPDX-License-Identifier: GPL-2.0-only
 * Change/restore a small RAM rectangle without ANY display register writes.
 * Guards compare live OVL3 and video mode. No DRM calls or y5disp triggers.
 */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
static volatile sig_atomic_t stop;
static void interrupted(int sig){(void)sig;stop=1;}
int main(int argc,char **argv){
 if(argc!=2||strcmp(argv[1],"--test-90s"))return 2;
 int hz=-1;FILE *f=fopen("/sys/kernel/y5disp/hz","r");if(!f)return 2;
 int ok=fscanf(f,"%d",&hz)==1;fclose(f);if(!ok||hz)return 2;
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *ovl=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x1400a000);
 volatile uint32_t *dsi=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x14012000);
 if(ovl==MAP_FAILED||dsi==MAP_FAILED)return 2;
 if(ovl[0x2c/4]!=8||ovl[0xfa0/4]!=0x775eb000||(ovl[0xa4/4]&65535)!=2880||ovl[0x98/4]!=0x05a002d0||(dsi[0x14/4]&3)!=1)return 3;
 volatile uint32_t *fb=mmap(NULL,0x3f4800,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x775eb000);if(fb==MAP_FAILED)return 2;
 uint32_t saved[180*60];
 signal(SIGINT,interrupted);signal(SIGTERM,interrupted);signal(SIGHUP,interrupted);
 for(unsigned y=0;y<60;y++)for(unsigned x=0;x<180;x++){unsigned p=(100+y)*720+80+x;saved[y*180+x]=fb[p];fb[p]=0xffff00ff;}
 __asm__ volatile("dsb sy":::"memory");
 puts("PATCH_VISIBLE_WINDOW=90s x=80 y=100 width=180 height=60 magenta; NO_DISPLAY_REGISTER_WRITES");fflush(stdout);
 for(unsigned i=0;i<90&&!stop;i++){struct timespec t={.tv_sec=1};nanosleep(&t,NULL);}
 for(unsigned y=0;y<60;y++)for(unsigned x=0;x<180;x++)fb[(100+y)*720+80+x]=saved[y*180+x];
 __asm__ volatile("dsb sy":::"memory");
 puts("PATCH_RESTORED; NO_DISPLAY_REGISTER_WRITES");
 munmap((void*)fb,0x3f4800);munmap((void*)ovl,4096);munmap((void*)dsi,4096);close(fd);return 0;
}
