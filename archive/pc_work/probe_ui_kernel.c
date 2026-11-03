#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(void){
 char name[64];snprintf(name,sizeof name,"/mfw-shm-probe-%ld",(long)getpid());
 int shared=shm_open(name,O_RDWR|O_CREAT|O_EXCL,0600);printf("shm_open fd=%d errno=%d %s\n",shared,shared<0?errno:0,shared<0?strerror(errno):"OK");if(shared>=0){close(shared);shm_unlink(name);}
 int fd=memfd_create("microfw-probe",MFD_CLOEXEC|MFD_ALLOW_SEALING);
 printf("memfd fd=%d errno=%d %s\n",fd,fd<0?errno:0,fd<0?strerror(errno):"OK");
 if(fd>=0){int r=ftruncate(fd,4096);printf("truncate=%d errno=%d\n",r,r<0?errno:0);r=fcntl(fd,F_ADD_SEALS,F_SEAL_SHRINK);printf("seal=%d errno=%d\n",r,r<0?errno:0);close(fd);}
 fd=epoll_create1(EPOLL_CLOEXEC);printf("epoll fd=%d errno=%d\n",fd,fd<0?errno:0);if(fd>=0)close(fd);
 return 0;
}
