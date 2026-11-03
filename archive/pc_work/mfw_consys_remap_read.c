/* SPDX-License-Identifier: GPL-2.0-only */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
int main(int argc,char **argv){
 if(argc!=2||strcmp(argv[1],"--read-only"))return 2;
 int fd=open("/dev/mem",O_RDONLY|O_SYNC|O_CLOEXEC);if(fd<0)return 1;
 void *mapping=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10001000);
 if(mapping==MAP_FAILED){close(fd);return 1;}
 volatile uint32_t *p=(volatile uint32_t *)((char *)mapping+0x380);
 uint32_t v=*p;
 printf("CONNSYS_EMI_REMAP address=10001380 value=%08x enable=%u encoded_base=%llx\n",v,!!(v&0x2000),(unsigned long long)(v&0x1fff)<<21);
 munmap(mapping,4096);close(fd);return 0;
}
