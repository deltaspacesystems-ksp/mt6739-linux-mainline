#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#define PACKED __attribute__((packed))
#define DRM_IOCTL_SRVKM_CMD 1
struct drm_srvkm_cmd {uint32_t bridge_id,bridge_func_id;uint64_t in_data_ptr,out_data_ptr;uint32_t in_data_size,out_data_size;};
static unsigned fault,events;
static uint32_t data[1024];
static int drmIoctl(int fd,unsigned request,void *arg){
 assert(fd==9&&request==1);struct drm_srvkm_cmd *cmd=arg;assert(cmd->bridge_id==6&&cmd->in_data_size==8);
 uint64_t handle=*(uint64_t*)(uintptr_t)cmd->in_data_ptr;uint8_t *out=(void*)(uintptr_t)cmd->out_data_ptr;
 if(cmd->bridge_func_id==3){events|=1;assert(handle==0x111&&cmd->out_data_size==12);if(fault==1)return -1;uint64_t ext=0x222;uint32_t error=0;__builtin_memcpy(out,&ext,8);__builtin_memcpy(out+8,&error,4);}
 else if(cmd->bridge_func_id==6){events|=2;assert(handle==0x222&&cmd->out_data_size==28);if(fault==2)return -1;uint64_t fields[3]={0x333,4096,4096};uint32_t error=0;__builtin_memcpy(out,fields,24);__builtin_memcpy(out+24,&error,4);}
 else if(cmd->bridge_func_id==7){events|=16;assert(handle==0x333&&cmd->out_data_size==4);*(uint32_t*)out=fault==6?5:0;}
 else{assert(cmd->bridge_func_id==4&&handle==0x222&&cmd->out_data_size==4);events|=32;*(uint32_t*)out=fault==7?5:0;}
 return 0;
}
static void *test_mmap(void *addr,size_t size,int prot,int flags,int fd,off_t offset){assert(!addr&&size==4096&&prot==PROT_READ&&flags==MAP_SHARED&&fd==9&&offset==0x333000);events|=4;return fault==3?MAP_FAILED:data;}
static int test_munmap(void *addr,size_t size){assert(addr==data&&size==4096);events|=8;return fault==5?-1:0;}
#define mmap test_mmap
#define munmap test_munmap
static int legacy_import_call(int fd,uint32_t function,uint64_t handle,void *out,uint32_t size){
 struct drm_srvkm_cmd cmd={.bridge_id=6,.bridge_func_id=function,.in_data_ptr=(uintptr_t)&handle,.out_data_ptr=(uintptr_t)out,.in_data_size=8,.out_data_size=size};
 if(!handle||(function!=3&&function!=4&&function!=6&&function!=7))return -1;
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
}
static int sync_readback(int fd,void *export_handle,uint32_t expected){
 struct {uint64_t handle;uint32_t error;} PACKED made={.error=UINT32_MAX};
 struct {uint64_t pmr,size,alignment;uint32_t error;} PACKED imported={.error=UINT32_MAX};
 _Static_assert(sizeof(made)==12&&sizeof(imported)==28,"1.9 local import ABI");
 volatile uint32_t *memory=MAP_FAILED;int rc=1;uint32_t error;
 if(legacy_import_call(fd,3,(uintptr_t)export_handle,&made,sizeof(made))||made.error||!made.handle)goto done;
 if(legacy_import_call(fd,6,made.handle,&imported,sizeof(imported))||imported.error||!imported.pmr)goto done;
 if(imported.size!=4096||imported.alignment!=4096||imported.pmr>INT64_MAX/4096)goto done;
 memory=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,(off_t)(imported.pmr*4096));
 if(memory==MAP_FAILED){perror("sync local-import mmap");goto done;}
 __sync_synchronize();
 if(memory[0]!=expected){fprintf(stderr,"sync readback mismatch got=%08x expected=%08x\n",memory[0],expected);goto done;}
 printf("MESA19_SYNC_READBACK_PASS value=%08x bytes=%llu; CPU readback, no GPU job\n",memory[0],(unsigned long long)imported.size);rc=0;
 done:
 if(memory!=MAP_FAILED&&munmap((void*)memory,4096)){rc=2;return rc;}
 if(imported.pmr){error=UINT32_MAX;if(legacy_import_call(fd,7,imported.pmr,&error,4)||error)return 2;}
 if(made.handle){error=UINT32_MAX;if(legacy_import_call(fd,4,made.handle,&error,4)||error)return 2;}
 return rc;
}
int main(void){
 unsigned masks[]={63,1,35,55,63,15,31,63};
 for(fault=0;fault<8;fault++){
  events=0;data[0]=fault==4?0:0x12345678;
  int rc=sync_readback(9,(void*)0x111,0x12345678);
  assert(rc==(fault==0?0:fault>=5?2:1));assert(events==masks[fault]);
 }
 puts("PASS exact sync import/readback cleanup: success plus7 injected failure paths");return 0;
}
