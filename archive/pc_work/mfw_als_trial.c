/* SPDX-License-Identifier: GPL-2.0-only */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
static volatile sig_atomic_t stop;
static void interrupted(int sig){(void)sig;stop=1;}
static int rd(int fd,uint8_t r,uint8_t *b,unsigned len){
 struct i2c_msg m[2]={{.addr=0x53,.len=1,.buf=&r},{.addr=0x53,.flags=I2C_M_RD,.len=len,.buf=b}};
 struct i2c_rdwr_ioctl_data t={.msgs=m,.nmsgs=2};return ioctl(fd,I2C_RDWR,&t)==2?0:-1;
}
static int control(int fd,uint8_t v){
 uint8_t b[2]={0,v};struct i2c_msg m={.addr=0x53,.len=2,.buf=b};
 struct i2c_rdwr_ioctl_data t={.msgs=&m,.nmsgs=1};return ioctl(fd,I2C_RDWR,&t)==1?0:-1;
}
static unsigned raw(const uint8_t b[3]){return b[0]|((unsigned)b[1]<<8)|((unsigned)b[2]<<16);}
int main(int argc,char **argv){
 if(argc!=2||strcmp(argv[1],"--als-only-two-seconds"))return 2;
 if(!access("/sys/bus/i2c/devices/1-0053/driver",F_OK))return 3;
 int fd=open("/dev/i2c-1",O_RDWR|O_CLOEXEC);if(fd<0){perror("open");return 3;}
 if(ioctl(fd,I2C_SLAVE,0x53)<0){close(fd);return 3;}
 uint8_t id,original,rate,gain,ctrl;int rc=1;
 if(rd(fd,6,&id,1)||id!=0xc2||rd(fd,0,&original,1)||original!=0||rd(fd,4,&rate,1)||rate!=0x22||rd(fd,5,&gain,1)||gain!=1)goto out;
 signal(SIGINT,interrupted);signal(SIGTERM,interrupted);signal(SIGHUP,interrupted);signal(SIGALRM,interrupted);alarm(5);
 if(control(fd,2)){perror("ALS enable");goto restore;}
 if(rd(fd,0,&ctrl,1)||ctrl!=2)goto restore;
 rc=0;
 for(unsigned i=0;i<8&&!stop;i++){
  struct timespec ts={.tv_nsec=250000000};nanosleep(&ts,NULL);
  uint8_t a[3],c[3];if(rd(fd,13,a,3)||rd(fd,10,c,3)){rc=1;break;}
  printf("ALS_RAW sample=%u als=%u channel0a=%u calibrated=false freshness_verified=false\n",i,raw(a),raw(c));
 }
restore:
 if(control(fd,0)||rd(fd,0,&ctrl,1)||ctrl!=0){fputs("RESTORE_FAILED\n",stderr);rc=1;}
 else puts("ALS_CONTROL_RESTORED=0; PS/IR never enabled");
 alarm(0);
out:
 close(fd);return rc;
}
