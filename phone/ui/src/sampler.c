#define _POSIX_C_SOURCE 200809L
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
