#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "mfw_kicksync_abi.h"
#define DRM_IOCTL_SRVKM_CMD 1
struct drm_srvkm_cmd {uint32_t bridge_id,bridge_func_id;uint64_t in_data_ptr,out_data_ptr;uint32_t in_data_size,out_data_size;};
static unsigned fault,events;static uint32_t memory;
static int mock_sleep(const struct timespec*a,struct timespec*b){(void)a;(void)b;return 0;}
#define nanosleep mock_sleep
static int drmIoctl(int fd,unsigned request,void *arg){
 assert(fd==9&&request==1);struct drm_srvkm_cmd*c=arg;assert(c->bridge_id==138);
 if(c->bridge_func_id==0){events|=1;assert(c->in_data_size==8&&c->out_data_size==12);PVRSRV_BRIDGE_IN_RGXCREATEKICKSYNCCONTEXT*i=(void*)(uintptr_t)c->in_data_ptr;assert(i->hPrivData==(void*)0x222);PVRSRV_BRIDGE_OUT_RGXCREATEKICKSYNCCONTEXT*o=(void*)(uintptr_t)c->out_data_ptr;o->hKickSyncContext=(void*)0x999;o->eError=fault==1?5:0;}
 else if(c->bridge_func_id==2){events|=2;assert(c->in_data_size==108&&c->out_data_size==8);PVRSRV_BRIDGE_IN_RGXKICKSYNC*i=(void*)(uintptr_t)c->in_data_ptr;assert(i->hKickSyncContext==(void*)0x999&&i->ui32ClientUpdateCount==1&&i->phUpdateUFOSyncPrimBlock[0]==(void*)0x777&&i->pui32UpdateSyncOffset[0]==0&&i->pui32UpdateValue[0]==0x87654321&&i->hCheckFenceFD==-1&&i->hTimelineFenceFD==-1&&i->ui32ClientFenceCount==0&&i->ui32ServerSyncCount==0);PVRSRV_BRIDGE_OUT_RGXKICKSYNC*o=(void*)(uintptr_t)c->out_data_ptr;o->eError=fault==2?5:0;o->hUpdateFenceFD=-1;if(!fault||fault==4)memory=i->pui32UpdateValue[0];}
 else{events|=4;assert(c->bridge_func_id==1&&c->in_data_size==8&&c->out_data_size==4);PVRSRV_BRIDGE_OUT_RGXDESTROYKICKSYNCCONTEXT*o=(void*)(uintptr_t)c->out_data_ptr;o->eError=fault==4?5:0;}
 return 0;
}
#include "mfw_kicksync_trial.h"
int main(void){for(fault=0;fault<5;fault++){events=0;memory=0x12345678;int rc=firmware_sync_trial(9,(void*)0x222,(void*)0x777,&memory);assert(rc==(fault==0?0:fault<3?1:2));assert(events==(fault==1?1:fault==3?3:7));}puts("PASS firmware-null diagnostic wire/cleanup/timeout mock tests");}
