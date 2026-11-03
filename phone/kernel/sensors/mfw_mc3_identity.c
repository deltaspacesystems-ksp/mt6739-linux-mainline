#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
static int read_reg(int fd,uint8_t reg,uint8_t *out){
 struct i2c_msg m[]={{.addr=0x4c,.len=1,.buf=&reg},{.addr=0x4c,.flags=I2C_M_RD,.len=1,.buf=out}};
 struct i2c_rdwr_ioctl_data t={.msgs=m,.nmsgs=2};return ioctl(fd,I2C_RDWR,&t)==2?0:-1;
}
int main(int argc,char **argv){
 if(argc!=2||strcmp(argv[1],"--read-only"))return 2;
 if(!access("/sys/bus/i2c/devices/1-004c/driver",F_OK))return 3;
 int fd=open("/dev/i2c-1",O_RDWR|O_CLOEXEC);if(fd<0)return 1;
 if(ioctl(fd,I2C_SLAVE,0x4c)<0){close(fd);return 1;}
 uint8_t product,hardware;int rc=1;
 if(!read_reg(fd,0x3b,&product)&&!read_reg(fd,0x18,&hardware)){
  printf("MC3XXX_ID product=%02x hardware=%02x; no configuration/calibration writes\n",product,hardware);rc=0;
 }else perror("MC3xxx identity");
 close(fd);return rc;
}
