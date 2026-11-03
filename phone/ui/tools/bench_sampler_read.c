#define _POSIX_C_SOURCE 200809L
#include "sampler.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static uint64_t ns(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
int main(void){struct mfw_model m={0};mfw_sample(&m);if(mfw_sampler_start(&m))return 1;uint64_t total=0,max=0;for(unsigned i=0;i<200;i++){uint64_t start=ns();mfw_sampler_read(&m);uint64_t dt=ns()-start;total+=dt;if(dt>max)max=dt;struct timespec t={.tv_nsec=1000000};nanosleep(&t,NULL);}mfw_sampler_stop();printf("SAMPLER_READ_BENCH n=200 avg_ns=%llu max_ns=%llu\n",(unsigned long long)(total/200),(unsigned long long)max);return 0;}
