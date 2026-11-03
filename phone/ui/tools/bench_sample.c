#define _POSIX_C_SOURCE 200809L
#include "ui.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static uint64_t ns(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
int main(void){struct mfw_model m={0};uint64_t total=0,min=UINT64_MAX,max=0;for(unsigned i=0;i<20;i++){uint64_t start=ns();mfw_sample(&m);uint64_t dt=ns()-start;total+=dt;if(dt<min)min=dt;if(dt>max)max=dt;}printf("SAMPLE_BENCH n=20 avg_us=%llu min_us=%llu max_us=%llu\n",(unsigned long long)(total/20/1000),(unsigned long long)(min/1000),(unsigned long long)(max/1000));return 0;}
