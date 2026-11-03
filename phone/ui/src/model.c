#define _POSIX_C_SOURCE 200809L
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int number(const char *p) {
    FILE *f=fopen(p,"r"); int n=-1;
    if (f) { if(fscanf(f,"%d",&n)!=1) n=-1; fclose(f); }
    return n;
}
static bool equals(const char *p,const char *value) {
    FILE *f=fopen(p,"r"); char buf[64]={0};
    if (!f) return false;
    if (!fgets(buf,sizeof buf,f)) buf[0]=0;
    fclose(f); buf[strcspn(buf,"\r\n")]=0;
    return strcmp(buf,value)==0;
}
void mfw_sample(struct mfw_model *m) {
    time_t now=time(NULL); struct tm tm;
    localtime_r(&now,&tm); strftime(m->clock,sizeof m->clock,"%H:%M",&tm);
    if (m->demo) {
        strcpy(m->clock,"9:41"); m->capacity=73; m->wifi=true;
        strcpy(m->battery_status,"On battery");
        m->charging=false; m->voltage_uv=3886000; m->temp_deci_c=291;
        m->cpu_hz=949999755; return;
    }
    m->capacity=number("/sys/class/power_supply/microfw-battery/capacity");
    if(m->capacity>100) m->capacity=-1;
    m->voltage_uv=number("/sys/class/power_supply/microfw-battery/voltage_now");
    m->temp_deci_c=number("/sys/class/power_supply/microfw-battery/temp");
    strcpy(m->battery_status,"Unknown");
    FILE *status=fopen("/sys/class/power_supply/microfw-battery/status","r");
    if(status){
        if(!fgets(m->battery_status,sizeof m->battery_status,status))strcpy(m->battery_status,"Unknown");
        fclose(status);m->battery_status[strcspn(m->battery_status,"\r\n")]=0;
    }
    m->charging=strcmp(m->battery_status,"Charging")==0;
    if(!strcmp(m->battery_status,"Discharging"))strcpy(m->battery_status,"On battery");
    m->wifi=equals("/sys/class/net/wlan0/operstate","up");
    int hz=number("/sys/bus/platform/devices/mfw-cpu-clock/cpu_rate_hz");
    m->cpu_hz=hz>0 ? (unsigned long)hz : 0;
}
