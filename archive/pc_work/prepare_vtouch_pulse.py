from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'y5_vtouch_snapshot.c').read_text()
s=s[:s.index('int main(void)')]
s=s.replace('MT6739 diagnostic reader.', 'MT6739 bounded touch power diagnostic.')
s=s.replace('No PMIC register writes: transport read-command/acknowledge only.', 'PMIC data writes are restricted to VLDO28 enable; no voltage writes.')
s=s.replace('Fixed VLDO28 registers only; no ADC sampling or reconfiguration.', 'Temporary VLDO28 enable; previous enable state must be restored.')
s=s.replace('#include <errno.h>','#define _POSIX_C_SOURCE 200809L\n#include <errno.h>')
s=s.replace('#include <unistd.h>','#include <unistd.h>\n#include <string.h>')
s+='''
/* Only the verified VLDO28 enable bit; voltage selector is never written. */
static int vtouch_enable(unsigned value)
{
 if(value>1 || wait_fsm(0))return -1;
 wr(0xc20, (1U<<31) | ((0x1b32U>>1)<<16) | value);
 return wait_fsm(0);
}
int main(int argc,char **argv)
{
 if(argc!=2 || strcmp(argv[1],"--pulse-vtouch"))return 2;
 if(!access("/sys/bus/platform/devices/1000d000.pwrap/driver",F_OK))return 2;
 if(!access("/sys/bus/platform/devices/10005000.gpio/driver",F_OK))return 2;
 int fd=open("/dev/mem",O_RDWR|O_SYNC);if(fd<0)return 2;
 volatile uint32_t *gpio=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,0x10005000);
 wrap=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x1000d000);
 if(gpio==MAP_FAILED || wrap==MAP_FAILED)return 2;
 /* RST119 must already be GPIO output LOW. Never change its mux or latch. */
 if((gpio[0x3e0/4]>>28)!=0 || !(gpio[0x30/4]&(1U<<23)) ||
    (gpio[0x130/4]&(1U<<23))){puts("RESET not held low; refusing");return 3;}
 unsigned id,vsel,before,enabled=0,after=0;
 if(pmic_read(0xa,&id)||id!=0x5721 || pmic_read(0x1c24,&vsel) ||
    (vsel&0x300)!=0x100 || pmic_read(0x1b32,&before) || before!=0){
   puts("Unexpected PMIC/voltage/enable; no data writes");return 3;
 }
 printf("VTOUCH before=0x%04x selector=0x%04x (2.8V), RST held LOW\\n",before,vsel);
 /* Mark the write as attempted even on timeout; cleanup must restore OFF. */
 int ret=vtouch_enable(1);
 if(!ret){struct timespec t={.tv_nsec=150000000};nanosleep(&t,NULL);
   ret=pmic_read(0x1b32,&enabled);if(!ret && enabled!=1)ret=-1;}
 int restore=vtouch_enable(before);
 if(!restore)restore=pmic_read(0x1b32,&after);
 if(!restore && after!=before)restore=-1;
 printf("VTOUCH enabled=0x%04x restored=0x%04x result=%d restore=%d\\n",enabled,after,ret,restore);
 munmap((void*)gpio,4096);munmap((void*)wrap,4096);close(fd);
 return restore?5:ret?4:0;
}
'''
(w/'y5_vtouch_pulse.c').write_text(s,newline='\n')
script=(w/'vtouch_snapshot.sh').read_text().replace('timeout 3 /tmp/y5_vtouch_snapshot','timeout 3 /var/cache/mfw-ui/y5_vtouch_pulse --pulse-vtouch')
(w/'vtouch_pulse.sh').write_text(script,newline='\n')
print('Prepared narrow, reversible VLDO28 pulse test; not executed')
