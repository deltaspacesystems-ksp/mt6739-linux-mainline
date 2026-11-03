/* SPDX-License-Identifier: GPL-2.0-only
 * Transitional MT6739 PIO -> uinput bridge, MStar stock demo43B format.
 * No PMIC mapping or controller data/firmware writes. Requires power lease.
 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
struct contact {int present,x,y,pressure;};
static volatile sig_atomic_t stop;
static void interrupted(int sig){(void)sig;stop=1;}
static uint64_t ns(void){struct timespec t;if(clock_gettime(CLOCK_MONOTONIC,&t))exit(2);return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
static void nap(void){struct timespec t={.tv_nsec=20000000};nanosleep(&t,NULL);}
static int decode(const unsigned char *p,size_t len,struct contact out[10]){
 memset(out,0,10*sizeof *out);if(len!=43||p[0]!=0x5a)return -1;
 unsigned sum=0;for(unsigned i=0;i<43;i++)sum=(sum+p[i])&255;if(sum)return -1;
 for(unsigned i=0;i<10;i++){const unsigned char *q=p+1+4*i;
  if(q[0]==255&&q[1]==255&&q[2]==255)continue;
  unsigned x=((q[0]&0xf0)<<4)|q[1],y=((q[0]&15)<<8)|q[2];
  if(x>2047||y>2047){memset(out,0,10*sizeof *out);return -1;}
  out[i]=(struct contact){1,(int)(x*720/2048),(int)(y*1440/2048),q[3]};
 }return 0;
}
static void checksum(unsigned char p[43]){unsigned sum=0;for(unsigned i=0;i<42;i++)sum+=p[i];p[42]=(unsigned char)(0-sum);}
static int selftest(void){unsigned char p[43];struct contact c[10];
 memset(p,255,sizeof p);p[0]=0x5a;p[41]=0;checksum(p);
 if(decode(p,43,c)){return 1;}for(unsigned i=0;i<10;i++)if(c[i].present)return 1;
 p[1]=0x24;p[2]=0xbe;p[3]=2;p[4]=6;checksum(p);
 if(decode(p,43,c)||!c[0].present||c[0].x!=246||c[0].y!=721||c[0].pressure!=6)return 1;
 p[37]=0x77;p[38]=255;p[39]=255;p[40]=9;checksum(p);
 if(decode(p,43,c)||!c[9].present||c[9].x!=719||c[9].y!=1439)return 1;
 if(!decode(p,42,c)){return 1;}p[42]^=1;if(!decode(p,43,c))return 1;
 p[0]=0;checksum(p);if(!decode(p,43,c))return 1;
 p[0]=0x5a;p[1]=0x84;checksum(p);if(!decode(p,43,c))return 1;
 puts("PASS: no contacts, captured contact, slot9 bounds, short/corrupt/header/out-of-range rejection");return 0;
}
static volatile uint16_t *bus;
static void bw(unsigned off,unsigned v){bus[off/2]=v;__asm__ volatile("dsb sy":::"memory");}
static int packet(unsigned char p[43]){
 const unsigned off[]={4,8,0x10,0x14,0x18,0x6c};unsigned saved[6],n=0,s=0;
 if((bus[0x24/2]&1)||((bus[0x30/2]>>4)&15))return -1;
 for(unsigned i=0;i<6;i++)saved[i]=bus[off[i]/2];
 bw(8,saved[1]&~7);bw(0xc,0x1ff);bw(0x38,1);bw(0x10,0x28);
 bw(4,(0x26<<1)|1);bw(0x14,43);bw(0x6c,0);bw(0x18,1);bw(0x24,1);
 uint64_t end=ns()+20000000;
 do{unsigned count=(bus[0x30/2]>>4)&15;
  while(count&&n<43){p[n++]=bus[0]&255;count=(bus[0x30/2]>>4)&15;}
  s=bus[0xc/2];if((s&6)||((s&1)&&(n==43||!count)))break;
 }while(ns()<end);
 bw(0xc,s);bw(0x38,1);for(unsigned i=0;i<6;i++)bw(off[i],saved[i]);
 return (s&7)==1&&n==43?0:-1;
}
static int axis(int fd,unsigned code,int max){
 struct uinput_abs_setup a={.code=(uint16_t)code,.absinfo={.minimum=0,.maximum=max}};
 return ioctl(fd,UI_SET_ABSBIT,code)<0||ioctl(fd,UI_ABS_SETUP,&a)<0?-1:0;
}
static int input_create(void){
 int fd=open("/dev/uinput",O_WRONLY|O_CLOEXEC);if(fd<0)return -1;
 if(ioctl(fd,UI_SET_EVBIT,EV_KEY)<0||ioctl(fd,UI_SET_KEYBIT,BTN_TOUCH)<0||
    ioctl(fd,UI_SET_EVBIT,EV_ABS)<0||ioctl(fd,UI_SET_PROPBIT,INPUT_PROP_DIRECT)<0||
    axis(fd,ABS_X,719)||axis(fd,ABS_Y,1439)||axis(fd,ABS_MT_SLOT,9)||
    axis(fd,ABS_MT_TRACKING_ID,65535)||axis(fd,ABS_MT_POSITION_X,719)||
    axis(fd,ABS_MT_POSITION_Y,1439)||axis(fd,ABS_MT_PRESSURE,255))goto fail;
 struct uinput_setup setup={.id={.bustype=BUS_VIRTUAL,.vendor=0,.product=1,.version=1}};
 snprintf(setup.name,sizeof setup.name,"MICROFW MStar Touchscreen");
 if(ioctl(fd,UI_DEV_SETUP,&setup)<0||ioctl(fd,UI_DEV_CREATE)<0)goto fail;
 char name[64]={0};if(ioctl(fd,UI_GET_SYSNAME(sizeof name),name)<0){ioctl(fd,UI_DEV_DESTROY);goto fail;}
 printf("UI_SYSNAME=%s\n",name);fflush(stdout);return fd;
fail:close(fd);return -1;
}
static struct input_event events[96];static unsigned ne;
static void event(unsigned type,unsigned code,int value){events[ne++]=(struct input_event){.type=(uint16_t)type,.code=(uint16_t)code,.value=value};}
static int emit(int fd,const struct contact c[10],int active[10],int *next){
 ne=0;unsigned count=0;int first=-1;
 for(unsigned i=0;i<10;i++){
  if(!c[i].present&&!active[i]){continue;}event(EV_ABS,ABS_MT_SLOT,(int)i);
  if(!c[i].present){event(EV_ABS,ABS_MT_TRACKING_ID,-1);active[i]=0;continue;}
  if(!active[i]){active[i]=*next;*next=*next>=65535?1:*next+1;event(EV_ABS,ABS_MT_TRACKING_ID,active[i]);}
  event(EV_ABS,ABS_MT_POSITION_X,c[i].x);event(EV_ABS,ABS_MT_POSITION_Y,c[i].y);event(EV_ABS,ABS_MT_PRESSURE,c[i].pressure);
  if(first<0){first=(int)i;}count++;
 }
 event(EV_KEY,BTN_TOUCH,count>0);if(first>=0){event(EV_ABS,ABS_X,c[first].x);event(EV_ABS,ABS_Y,c[first].y);}event(EV_SYN,SYN_REPORT,0);
 size_t size=ne*sizeof *events;unsigned char *p=(unsigned char*)events;
 while(size){ssize_t n=write(fd,p,size);if(n<0&&errno==EINTR)continue;if(n<=0)return -1;p+=n;size-=(size_t)n;}return 0;
}
int main(int argc,char **argv){
 setvbuf(stdout,NULL,_IOLBF,0);
 if(argc==2&&!strcmp(argv[1],"--selftest"))return selftest();
 if(argc!=3||strcmp(argv[1],"--seconds"))return 2;
 char *end;long seconds=strtol(argv[2],&end,10);if(*end||seconds<1||seconds>600)return 2;
 if(access("/run/mfw-touch.lock/active",F_OK)||access("/sys/bus/platform/devices/1000d000.pwrap/driver",F_OK))return 3;
 if(!access("/sys/bus/platform/devices/11007000.i2c/driver",F_OK)||!access("/sys/bus/platform/devices/10005000.gpio/driver",F_OK))return 3;
 int mem=open("/dev/mem",O_RDWR|O_SYNC);if(mem<0)return 3;
 volatile uint32_t *gpio=mmap(NULL,4096,PROT_READ,MAP_SHARED,mem,0x10005000);
 volatile uint32_t *infra=mmap(NULL,4096,PROT_READ,MAP_SHARED,mem,0x10001000);
 if(gpio==MAP_FAILED||infra==MAP_FAILED||(infra[0x90/4]&(1U<<11)))return 3;
 bus=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,mem,0x11007000);if(bus==MAP_FAILED)return 3;
 int fd=input_create();if(fd<0){perror("uinput create");return 4;}
 signal(SIGINT,interrupted);signal(SIGTERM,interrupted);signal(SIGHUP,interrupted);
 uint64_t until=ns()+(uint64_t)seconds*1000000000ULL,last_valid=ns();unsigned good=0,bad=0,empty=0,high_good=0,low_good=0;int active[10]={0},next=1,ret=0,last_count=-1;unsigned bus_errors=0,invalid_reports=0;uint64_t longest_contact_gap=0;
 while(!stop&&ns()<until){
  /* Until pad input/IRQ configuration is ported, do not assume that a
   * sampled GPIO level identifies every complete frame. Poll normal data
   * at most50Hz and validate the entire packet before emitting anything. */
  {int irq_high=!!(gpio[0x200/4]&(1U<<6));unsigned char p[43]={0};struct contact c[10];
   int bus_error=packet(p);int invalid=bus_error?0:decode(p,43,c);
   if(bus_error||invalid){
    if(bus_error)bus_errors++;else invalid_reports++;
    bad++;
    /* Invalid packets never synthesize a finger-up followed by another tap.
     * A persistent failure during contact closes this bounded reader once. */
    int held=0;for(unsigned i=0;i<10;i++)held|=active[i]!=0;
    if(held&&ns()-last_valid>2000000000ULL){fputs("INPUT_TIMEOUT: closing reader\n",stderr);ret=6;break;}
   }
   else{
    uint64_t now=ns();int was_held=0;for(unsigned i=0;i<10;i++)was_held|=active[i]!=0;
    if(was_held&&now-last_valid>longest_contact_gap)longest_contact_gap=now-last_valid;
    good++;last_valid=now;if(irq_high)high_good++;else low_good++;
    int count=0;for(unsigned i=0;i<10;i++)count+=c[i].present;
    if(!count)empty++;
    if(count!=last_count){printf("CONTACT_COUNT=%d irq_high=%d good=%u bad=%u monotonic_ms=%llu\n",count,irq_high,good,bad,(unsigned long long)(last_valid/1000000));last_count=count;}
    if(emit(fd,c,active,&next)){ret=5;break;}
   }
  }nap();
 }
 struct contact released[10]={0};if(emit(fd,released,active,&next))ret=5;
 ioctl(fd,UI_DEV_DESTROY);close(fd);munmap((void*)bus,4096);munmap((void*)gpio,4096);munmap((void*)infra,4096);close(mem);
 printf("TOUCH_TIMING bus_errors=%u invalid_reports=%u longest_contact_gap_us=%llu\n",bus_errors,invalid_reports,(unsigned long long)(longest_contact_gap/1000));
 printf("TOUCH_READER_EXIT good=%u bad=%u empty=%u high_good=%u low_good=%u result=%d\n",good,bad,empty,high_good,low_good,ret);return ret;
}
