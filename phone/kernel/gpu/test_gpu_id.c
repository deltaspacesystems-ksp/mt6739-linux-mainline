/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-gpu-id.h"
int main(void){struct mfw_gpu_id id;
 assert(!mfw_gpu_decode_id(0,22ULL<<48|49ULL<<32|21ULL<<16|16,&id));assert(id.b==22&&id.v==49&&id.n==21&&id.c==16);
 assert(!mfw_gpu_decode_id(21<<8|16,22<<16|49<<8,&id));assert(id.b==22&&id.v==49&&id.n==21&&id.c==16);
 assert(mfw_gpu_decode_id(0,0,&id)==-ENODEV);assert(mfw_gpu_decode_id(~0U,~0ULL,&id)==-ENODEV);
 puts("PASS: PBVNC/legacy register decode, zero/all-ones rejection; synthetic cases not hardware evidence");return 0;}
