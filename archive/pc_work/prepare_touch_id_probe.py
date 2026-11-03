from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'y5_vtouch_snapshot.c').read_text().split('int main(void)')[0]
s=s.replace('MT6739 diagnostic reader.', 'MT6739 bounded touch reset/ID diagnostic.')
s=s.replace('No PMIC register writes: transport read-command/acknowledge only.', 'PMIC data writes are restricted to VLDO28 enable; no voltage writes.')
s=s.replace('Fixed VLDO28 registers only; no ADC sampling or reconfiguration.', 'Temporary power/reset changes; previous enable/GPIO state restored.')
s=s.replace('#include <errno.h>','#define _POSIX_C_SOURCE 200809L\n#include <errno.h>\n#include <string.h>')
s+='''
static volatile uint32_t *gpio,*infra;
static volatile uint16_t *bus;
static void pause_ms(long ms){struct timespec t={.tv_sec=ms/1000,.tv_nsec=(ms%1000)*1000000};while(nanosleep(&t,&t)&&errno==EINTR){}}
static void gpio_wr(unsigned off,uint32_t v){gpio[off/4]=v;__asm__ volatile("dsb sy":::"memory");}
static void pin_out(unsigned pin,int value){unsigned group=(pin/32)*16,bit=pin%32;gpio_wr(0x100+group+(value?4:8),1U<<bit);}
static void pin_dir(unsigned pin,int output){gpio_wr((pin/32)*16+(output?4:8),1U<<(pin%32));}
static void mode6(unsigned mode){uint32_t v=gpio[0x300/4];gpio_wr(0x300,(v&~(15U<<24))|(mode<<24));}
static int vtouch_enable(unsigned value){if(value>1||wait_fsm(0))return -1;wr(0xc20,(1U<<31)|((0x1b32U>>1)<<16)|value);return wait_fsm(0);}
static void bw(unsigned off,unsigned v){bus[off/2]=v;__asm__ volatile("dsb sy":::"memory");}
static int read_product(unsigned char data[6])
{
 const unsigned offsets[]={4,8,0x10,0x14,0x18,0x6c};unsigned saved[6],status=0;int ret=-1;
 if((bus[0x24/2]&1)||((bus[0x30/2]>>4)&15))return -2;
 for(unsigned i=0;i<6;i++)saved[i]=bus[offsets[i]/2];
 bw(8,saved[1]&~7);bw(0xc,0x1ff);bw(0x38,1);bw(0x10,0x3a);
 bw(4,0x5d<<1);bw(0x14,2);bw(0x6c,6);bw(0x18,2);
 bw(0,0x81);bw(0,0x40);bw(0x24,1);
 uint64_t until=now_ns()+20000000;
 do{status=bus[0xc/2];if(status&7)break;}while(now_ns()<until);
 unsigned count=(bus[0x30/2]>>4)&15;
 printf("I2C0 status=0x%04x rx_count=%u start=%u\\n",status,count,bus[0x24/2]&1);
 if((status&7)==1 && count==6){for(unsigned i=0;i<6;i++)data[i]=bus[0]&255;ret=0;}
 bw(0xc,status);bw(0x38,1);
 for(unsigned i=0;i<6;i++)bw(offsets[i],saved[i]);
 return ret;
}
int main(int argc,char **argv)
{
 if(argc!=2||strcmp(argv[1],"--probe-id"))return 2;
 const char *owners[]={"/sys/bus/platform/devices/1000d000.pwrap/driver","/sys/bus/platform/devices/10005000.gpio/driver","/sys/bus/platform/devices/11007000.i2c/driver"};
 for(unsigned i=0;i<3;i++)if(!access(owners[i],F_OK)){puts("Resource owned; refusing");return 2;}
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 gpio=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x10005000);
 infra=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10001000);
 wrap=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x1000d000);
 if(gpio==MAP_FAILED||infra==MAP_FAILED||wrap==MAP_FAILED)return 2;
 if(infra[0x90/4]&(1U<<11)){puts("I2C0 gated; no changes");return 3;}
 bus=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x11007000);if(bus==MAP_FAILED)return 2;
 unsigned id,vsel,before;
 if(pmic_read(0xa,&id)||id!=0x5721||pmic_read(0x1c24,&vsel)||(vsel&0x300)!=0x100||pmic_read(0x1b32,&before)||before!=0)return 3;
 if((gpio[0x3e0/4]>>28)!=0||!(gpio[0x30/4]&(1U<<23))||(gpio[0x130/4]&(1U<<23)))return 3;
 if((bus[0x24/2]&1)||((bus[0x30/2]>>4)&15)){puts("I2C0 busy; no changes");return 3;}
 unsigned oldmode=(gpio[0x300/4]>>24)&15,olddir=(gpio[0]>>6)&1,oldout=(gpio[0x100/4]>>6)&1;
 printf("TOUCH preflight vsel=0x%04x reset119=LOW int6 mode=%u dir=%u out=%u\\n",vsel,oldmode,olddir,oldout);
 int ret=vtouch_enable(1);unsigned enabled=0,after=0;
 if(!ret)ret=pmic_read(0x1b32,&enabled);
 if(!ret&&enabled!=1)ret=-1;
 unsigned char data[6]={0};
 if(!ret){
   /* Stock: power on with RST/INT low, wait100ms; select0x5d,
    * RST low20ms, INT low20ms, RST high10ms, INT low58ms, input.
    * No watchdog/config/firmware writes to the touch controller. */
   pin_out(6,0);mode6(0);pin_dir(6,1);pause_ms(100);
   pin_out(119,0);pause_ms(20);pin_out(6,0);pause_ms(20);
   pin_out(119,1);pause_ms(10);pin_out(6,0);pause_ms(58);pin_dir(6,0);pause_ms(20);
   ret=read_product(data);
 }
 pin_out(119,0);pause_ms(20);
 pin_out(6,oldout);pin_dir(6,olddir);mode6(oldmode);
 int restore=vtouch_enable(before);if(!restore)restore=pmic_read(0x1b32,&after);
 if(!restore&&after!=before)restore=-1;
 printf("TOUCH data=%02x %02x %02x %02x %02x %02x result=%d restore=%d enable_after=%u\\n",data[0],data[1],data[2],data[3],data[4],data[5],ret,restore,after);
 munmap((void*)bus,4096);munmap((void*)gpio,4096);munmap((void*)infra,4096);munmap((void*)wrap,4096);close(fd);
 return restore?5:ret?4:0;
}
'''
(w/'y5_touch_id_probe.c').write_text(s,newline='\n')
script=(w/'vtouch_snapshot.sh').read_text().replace('timeout 3 /tmp/y5_vtouch_snapshot','timeout 3 /var/cache/mfw-ui/y5_touch_id_probe --probe-id')
(w/'touch_id_probe.sh').write_text(script,newline='\n')
print('Prepared bounded ID probe, no touch firmware/config writes')
