from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'y5_mstar_frame_probe.c').read_text().split('static int read_product(')[0]
s=s[:s.index('/* Stock demo mode:')]
s=s.replace('#include <string.h>','#include <string.h>\n#include <sys/stat.h>')
s+='''
#define ACTIVE "/run/mfw-touch.lock/active"
#define MAGIC "mfw-mstar-touch-v1\\n"
static int state_valid(void){char value[64]={0};struct stat st;int f=open(ACTIVE,O_RDONLY|O_NOFOLLOW);if(f<0)return 0;
 int ok=!fstat(f,&st)&&S_ISREG(st.st_mode)&&st.st_uid==0&&(st.st_mode&0777)==0600;
 ssize_t n=read(f,value,sizeof value-1);close(f);return ok&&n==(ssize_t)strlen(MAGIC)&&!strcmp(value,MAGIC);}
int main(int argc,char **argv){
 if(argc!=2){return 2;}int on=!strcmp(argv[1],"--on"),off=!strcmp(argv[1],"--off");if(!on&&!off)return 2;
 if(!access("/sys/bus/platform/devices/1000d000.pwrap/driver",F_OK)||!access("/sys/bus/platform/devices/10005000.gpio/driver",F_OK)||!access("/sys/bus/platform/devices/11007000.i2c/driver",F_OK))return 2;
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 gpio=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x10005000);
 wrap=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x1000d000);
 if(gpio==MAP_FAILED||wrap==MAP_FAILED)return 2;
 unsigned id,vsel,en,after;
 if(pmic_read(0xa,&id)||id!=0x5721||pmic_read(0x1c24,&vsel)||(vsel&0x300)!=0x100||pmic_read(0x1b32,&en)||en>1)return 3;
 if((gpio[0x3e0/4]>>28)!=0||!(gpio[0x30/4]&(1U<<23)))return 3;
 if(on){
   if(en||gpio[0x130/4]&(1U<<23)||((gpio[0x300/4]>>24)&15)||gpio[0]&(1U<<6)||gpio[0x100/4]&(1U<<6))return 3;
   int f=open(ACTIVE,O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW,0600);if(f<0)return 3;
   if(write(f,MAGIC,strlen(MAGIC))!=(ssize_t)strlen(MAGIC)){close(f);unlink(ACTIVE);return 3;}close(f);
   if(vtouch_enable(1)||pmic_read(0x1b32,&after)||after!=1)return 4;
   pin_out(6,0);mode6(0);pin_dir(6,1);pause_ms(50);
   pin_out(119,1);pause_ms(10);pin_out(119,0);pause_ms(10);pin_out(119,1);pause_ms(25);
   pin_dir(6,0);pause_ms(100);puts("TOUCH_POWER_ON lease created; rebind PMIC before reader");
 }else{
   if(!state_valid())return 3;
   pin_out(119,0);pause_ms(20);pin_out(6,0);pin_dir(6,0);mode6(0);
   if(vtouch_enable(0)||pmic_read(0x1b32,&after)||after)return 5;
   if(unlink(ACTIVE)){return 5;}puts("TOUCH_POWER_OFF restored verified initial state");
 }
 munmap((void*)gpio,4096);munmap((void*)wrap,4096);close(fd);return 0;
}
'''
# Reader-only globals/helpers are not used by this short power tool.
s=s.replace('static volatile uint32_t *gpio,*infra;','static volatile uint32_t *gpio;')
s=s.replace('static volatile uint16_t *bus;','')
s=s[:s.index('static void bw(')]+s[s.index('#define ACTIVE'):]
(w/'y5_mstar_touch_power.c').write_text(s,newline='\n')
print('Prepared short power/restore helper, lease guarded')
