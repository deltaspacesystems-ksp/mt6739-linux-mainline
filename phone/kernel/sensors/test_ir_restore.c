#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
typedef uint8_t u8;
struct device {int unused;};struct device_attribute {int unused;};
struct {int adapter;} bus_data,*bus=&bus_data;
#define I2C_LOCK_ROOT_ADAPTER 0
static bool ir_test=true,ir_restored,faulted;
static int ir_result,calls,failcall,locked,sleeps;
static unsigned ir_samples;
static unsigned short ir_values[50];
static u8 registers[4]={0,0x36,8,0x45};
static void i2c_lock_bus(int *a,int mode){(void)a;(void)mode;assert(!locked);locked=1;}
static void i2c_unlock_bus(int *a,int mode){(void)a;(void)mode;assert(locked);locked=0;}
static bool sysfs_streq(const char *a,const char *b){return !strcmp(a,b);}
static void msleep(unsigned ms){assert(ms==100);sleeps++;}
static int ir_transfer_read(u8 r,u8 *data,unsigned len){
 assert(locked);if(++calls==failcall)faulted=true;if(faulted)return -EIO;
 if(r==6){assert(len==1);*data=0xc2;}else if(r==8){assert(len==2);data[0]=42;data[1]=1;}else{assert(r<4&&len==1);*data=registers[r];}return 0;
}
static int ir_transfer_write(u8 r,u8 value){
 assert(locked&&r<4);if(++calls==failcall)faulted=true;
 if(faulted&&!(r==0&&value==0))return -EIO;
 registers[r]=value;return 0;
}
static ssize_t ir_trial_store(struct device *dev,struct device_attribute *attr,const char *buf,size_t count)
{
 u8 original[4],id,data[2],check;unsigned i,saved=0;int rc=0,restore=0;
 if(!ir_test||!sysfs_streq(buf,"run"))return -EPERM;
 i2c_lock_bus(&bus->adapter,I2C_LOCK_ROOT_ADAPTER);
 ir_samples=0;ir_restored=false;ir_result=-EINPROGRESS;
 rc=ir_transfer_read(6,&id,1);
 if(rc||id!=0xc2){if(!rc)rc=-ENODEV;goto done;}
 for(i=0;i<4;i++){rc=ir_transfer_read(i,&original[i],1);if(rc)goto done;saved++;}
 if(original[0]){rc=-EBUSY;goto done;}
 /* Exact stock4.4 PS defaults, including the non-b1 path. No IRQ/NVRAM. */
 rc=ir_transfer_write(1,0x36);if(rc)goto cleanup;
 rc=ir_transfer_write(2,0x10);if(rc)goto cleanup;
 rc=ir_transfer_write(3,0x5c);if(rc)goto cleanup;
 rc=ir_transfer_write(0,1);if(rc)goto cleanup;
 for(i=0;i<50;i++){
  msleep(100);
  rc=ir_transfer_read(8,data,2);if(rc)break;
  ir_values[ir_samples++]=data[0]|((data[1]&7)<<8);
 }
cleanup:
 /* This callback completes restoration even if the writing process exits. */
 if(ir_transfer_write(0,0))restore=-EIO;
 for(i=3;i>0;i--)if(ir_transfer_write(i,original[i]))restore=-EIO;
 for(i=0;i<4;i++){
  int rr=ir_transfer_read(i,&check,1);
  if(rr||check!=original[i])restore=-EIO;
 }
 ir_restored=!restore;
 if(restore)rc=restore;
done:
 ir_result=rc;
 if(saved<4)ir_restored=false;
 i2c_unlock_bus(&bus->adapter,I2C_LOCK_ROOT_ADAPTER);
 if(rc)return rc;
 return count;
}

int main(void){
 ssize_t result=ir_trial_store(NULL,NULL,"run",3);
 assert(result==3&&ir_result==0&&ir_restored&&ir_samples==50&&sleeps==50&&registers[0]==0&&registers[1]==0x36&&registers[2]==8&&registers[3]==0x45&&!locked);
 int total=calls;
 for(int f=1;f<=total;f++){
  registers[0]=0;registers[1]=0x36;registers[2]=8;registers[3]=0x45;
  calls=0;failcall=f;faulted=false;sleeps=0;
  assert(ir_trial_store(NULL,NULL,"run",3)<0&&!locked);
  assert(registers[0]==0); /* shutdown command remains allowed after fault */
 }
 failcall=0;faulted=false;registers[0]=2;assert(ir_trial_store(NULL,NULL,"run",3)==-EBUSY&&registers[0]==2&&!locked);
 ir_test=false;assert(ir_trial_store(NULL,NULL,"run",3)==-EPERM&&!locked);
 printf("PASS exact IR callback normal restore, %d fault positions, busy/opt-in guards and bounded PS-off\n",total);
}
