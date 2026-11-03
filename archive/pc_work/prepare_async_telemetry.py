from pathlib import Path
w=Path(r'D:\y5_linux_work\microfw-ui')
(w/'src/sampler.h').write_text('''#ifndef MFW_SAMPLER_H
#define MFW_SAMPLER_H
#include "ui.h"
/* One sampler per process. UI thread owns start/read/stop; only telemetry
 * crosses the mutex, never Wayland/Cairo objects or window/gesture state. */
int mfw_sampler_start(const struct mfw_model *initial);
void mfw_sampler_read(struct mfw_model *model);
void mfw_sampler_stop(void);
#endif
''',newline='\n')
(w/'src/sampler.c').write_text('''#define _POSIX_C_SOURCE 200809L
#include "sampler.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
static pthread_t worker;
static pthread_mutex_t lock;
static pthread_cond_t wake;
static struct mfw_model snapshot;
static bool started, stopping;
static void copy_telemetry(struct mfw_model *dst,const struct mfw_model *src){
    memcpy(dst->clock,src->clock,sizeof dst->clock);
    memcpy(dst->battery_status,src->battery_status,sizeof dst->battery_status);
    dst->capacity=src->capacity;dst->voltage_uv=src->voltage_uv;
    dst->temp_deci_c=src->temp_deci_c;dst->cpu_hz=src->cpu_hz;
    dst->charging=src->charging;dst->wifi=src->wifi;
}
static void *sample_worker(void *unused){
    (void)unused;struct mfw_model next={.demo=snapshot.demo};
    for(;;){
        /* Slow sysfs/PMIC reads MUST remain outside the snapshot lock. */
        mfw_sample(&next);
        pthread_mutex_lock(&lock);
        copy_telemetry(&snapshot,&next);
        struct timespec deadline;clock_gettime(CLOCK_MONOTONIC,&deadline);deadline.tv_sec++;
        if(!stopping)pthread_cond_timedwait(&wake,&lock,&deadline);
        bool quit=stopping;pthread_mutex_unlock(&lock);
        if(quit)return NULL;
    }
}
int mfw_sampler_start(const struct mfw_model *initial){
    if(started)return EALREADY;
    int err=pthread_mutex_init(&lock,NULL);if(err)return err;
    pthread_condattr_t attr;err=pthread_condattr_init(&attr);
    if(err){pthread_mutex_destroy(&lock);return err;}
    err=pthread_condattr_setclock(&attr,CLOCK_MONOTONIC);
    if(!err)err=pthread_cond_init(&wake,&attr);
    pthread_condattr_destroy(&attr);
    if(err){pthread_mutex_destroy(&lock);return err;}
    snapshot=*initial;stopping=false;
    err=pthread_create(&worker,NULL,sample_worker,NULL);
    if(err){pthread_cond_destroy(&wake);pthread_mutex_destroy(&lock);return err;}
    started=true;return 0;
}
void mfw_sampler_read(struct mfw_model *model){
    if(!started)return;
    pthread_mutex_lock(&lock);copy_telemetry(model,&snapshot);pthread_mutex_unlock(&lock);
}
void mfw_sampler_stop(void){
    if(!started)return;
    pthread_mutex_lock(&lock);stopping=true;pthread_cond_signal(&wake);pthread_mutex_unlock(&lock);
    pthread_join(worker,NULL);pthread_cond_destroy(&wake);pthread_mutex_destroy(&lock);started=false;
}
''',newline='\n')
for name in ['desktop.c','wayland.c','main.c']:
 p=w/'src'/name;s=p.read_text();s=s.replace('#include "ui.h"','#include "ui.h"\n#include "sampler.h"',1)
 if name!='main.c':s=s.replace('mfw_sample(model);','mfw_sampler_read(model);')
 else:s=s.replace('if(!preview)return desktop?mfw_desktop(&m,frames,smoke):mfw_wayland(&m,windowed,frames);','''if(!preview){
        int err=mfw_sampler_start(&m);
        if(err){fprintf(stderr,"Telemetry worker: %s\\n",strerror(err));return 1;}
        int result=desktop?mfw_desktop(&m,frames,smoke):mfw_wayland(&m,windowed,frames);
        mfw_sampler_stop();return result;
    }''')
 p.write_text(s,newline='\n')
p=w/'Makefile';s=p.read_text().replace('src/main.c src/model.c','src/main.c src/model.c src/sampler.c').replace('LDLIBS += $(shell','LDLIBS += -pthread $(shell',1).replace('CFLAGS += -std=c11','CFLAGS += -pthread -std=c11',1);p.write_text(s,newline='\n')
print('Moved periodic telemetry outside UI threads; initial startup sample preserved')
