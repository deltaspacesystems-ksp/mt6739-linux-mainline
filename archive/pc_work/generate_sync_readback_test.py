from pathlib import Path
s=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c').read_text();a=s.index('static int legacy_import_call');b=s.index('int main(void){',a)
prefix='''#define _GNU_SOURCE
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
'''
tail='''int main(void){
 unsigned masks[]={63,1,35,55,63,15,31,63};
 for(fault=0;fault<8;fault++){
  events=0;data[0]=fault==4?0:0x12345678;
  int rc=sync_readback(9,(void*)0x111,0x12345678);
  assert(rc==(fault==0?0:fault>=5?2:1));assert(events==masks[fault]);
 }
 puts("PASS exact sync import/readback cleanup: success plus7 injected failure paths");return 0;
}
'''
Path('/mnt/d/y5_linux_work/test_sync_readback.c').write_text(prefix+s[a:b]+tail)
