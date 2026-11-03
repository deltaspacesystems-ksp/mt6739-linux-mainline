#define _POSIX_C_SOURCE 200809L
#include "sampler.h"
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
static pthread_mutex_t gate=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed=PTHREAD_COND_INITIALIZER;
static int entered,released;
void mfw_sample(struct mfw_model *m){
 pthread_mutex_lock(&gate);entered=1;pthread_cond_broadcast(&changed);
 while(!released)pthread_cond_wait(&changed,&gate);
 pthread_mutex_unlock(&gate);
 m->capacity=77;m->temp_deci_c=303;strcpy(m->clock,"12:34");
}
int main(void){
 alarm(5);
 struct mfw_model m={.capacity=11,.page=PAGE_SWITCHER,.scale=.75,.n_windows=1};
 m.windows[0].id=42;strcpy(m.windows[0].title,"Preserve window state");
 assert(!mfw_sampler_start(&m));assert(mfw_sampler_start(&m)==EALREADY);
 pthread_mutex_lock(&gate);while(!entered)pthread_cond_wait(&changed,&gate);pthread_mutex_unlock(&gate);
 /* Worker deliberately blocked in a slow sample: reads must still return. */
 for(int i=0;i<1000;i++){mfw_sampler_read(&m);assert(m.capacity==11);}
 pthread_mutex_lock(&gate);released=1;pthread_cond_broadcast(&changed);pthread_mutex_unlock(&gate);
 for(int i=0;i<1000&&m.capacity!=77;i++){struct timespec t={.tv_nsec=1000000};nanosleep(&t,NULL);mfw_sampler_read(&m);}
 assert(m.capacity==77&&m.temp_deci_c==303&&!strcmp(m.clock,"12:34"));
 assert(m.page==PAGE_SWITCHER&&m.scale==.75&&m.n_windows==1&&m.windows[0].id==42&&!strcmp(m.windows[0].title,"Preserve window state"));
 mfw_sampler_stop();mfw_sampler_stop();assert(!mfw_sampler_start(&m));mfw_sampler_stop();
 puts("PASS: UI reads while telemetry blocked, coherent publication, window state preservation, start/stop lifecycle");return 0;
}
