from pathlib import Path
p=Path('D:/y5_linux_work/mfw-i2c1-probe/mfw_i2c1_probe.c');s=p.read_text();s=s.replace(' if (b->faulted) return -EIO;', ' /* A latched bus permits one bounded best-effort PS-off command only. */\n if (b->faulted && !(internal && n==1 && m[0].buf[0]==0 && m[0].buf[1]==0)) return -EIO;');s=s.replace(' if (ret) {\n  b->faulted = true;\n  __module_get(THIS_MODULE);',' if (ret) {\n  if(!b->faulted)__module_get(THIS_MODULE);\n  b->faulted = true;');p.write_text(s,newline='\n')
a=s.index('static ssize_t ir_trial_store(');z=s.index('static DEVICE_ATTR_RW',a)
prefix='''#include <assert.h>
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
'''
body=s[a:z]
suffix='''
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
 printf("PASS exact IR callback normal restore, %d fault positions, busy/opt-in guards and bounded PS-off\\n",total);
}
'''
Path('D:/y5_linux_work/mfw-i2c1-probe/test_ir_restore.c').write_text(prefix+body+suffix,newline='\n')
