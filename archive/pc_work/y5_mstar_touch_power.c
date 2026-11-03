/* SPDX-License-Identifier: GPL-2.0-only
 * MT6739 bounded touch reset/ID diagnostic. WACS2 offsets verified against stock 4.4.
 * PMIC data writes are restricted to VLDO28 enable; no voltage writes.
 * Temporary power/reset changes; previous enable/GPIO state restored.
 * Run only while no kernel driver owns PWRAP. The command and acknowledge
 * writes below affect the wrapper transport, not PMIC register contents.
 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

static volatile uint32_t *wrap;
static uint64_t now_ns(void)
{
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t)) exit(2);
    return (uint64_t)t.tv_sec * 1000000000ULL + t.tv_nsec;
}
static uint32_t rd(unsigned off) { return wrap[off / 4]; }
static void wr(unsigned off, uint32_t v)
{
    wrap[off / 4] = v;
    __asm__ volatile("dsb sy" ::: "memory");
}
static int wait_fsm(unsigned wanted)
{
    uint64_t end = now_ns() + 20000000;
    do {
        uint32_t s = rd(0xc24);
        if (!(s & (1U << 21))) return -1;
        if (((s >> 16) & 7) == wanted) return 0;
    } while (now_ns() < end);
    return -1;
}
static int pmic_read(unsigned reg, unsigned *value)
{
    if (wait_fsm(0)) return -1;
    wr(0xc20, (reg >> 1) << 16); /* bit31=0: read only */
    if (wait_fsm(6)) return -1;
    *value = rd(0xc24) & 0xffff;
    wr(0xc28, 1);
    return wait_fsm(0);
}

static volatile uint32_t *gpio;

static void pause_ms(long ms){struct timespec t={.tv_sec=ms/1000,.tv_nsec=(ms%1000)*1000000};while(nanosleep(&t,&t)&&errno==EINTR){}}
static void gpio_wr(unsigned off,uint32_t v){gpio[off/4]=v;__asm__ volatile("dsb sy":::"memory");}
static void pin_out(unsigned pin,int value){unsigned group=(pin/32)*16,bit=pin%32;gpio_wr(0x100+group+(value?4:8),1U<<bit);}
static void pin_dir(unsigned pin,int output){gpio_wr((pin/32)*16+(output?4:8),1U<<(pin%32));}
static void mode6(unsigned mode){uint32_t v=gpio[0x300/4];gpio_wr(0x300,(v&~(15U<<24))|(mode<<24));}
static int vtouch_enable(unsigned value){if(value>1||wait_fsm(0))return -1;wr(0xc20,(1U<<31)|((0x1b32U>>1)<<16)|value);return wait_fsm(0);}
#define ACTIVE "/run/mfw-touch.lock/active"
#define MAGIC "mfw-mstar-touch-v1\n"
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
