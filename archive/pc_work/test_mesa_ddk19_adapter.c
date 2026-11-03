#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <xf86drm.h>
#define PACKED __attribute__((packed))
#define PVR_MFW_DDK19 1
typedef struct {uint64_t addr;} pvr_dev_addr_t;
#define PVR_SRV_BRIDGE_SRVCORE 1UL
#define PVR_SRV_BRIDGE_SRVCORE_CONNECT 0UL
#define PVR_SRV_BRIDGE_SRVCORE_DISCONNECT 1UL
#define PVR_SRV_BRIDGE_MM 6UL
#define PVR_SRV_BRIDGE_MM_DEVMEMINTCTXCREATE 15UL
#define PVR_SRV_BRIDGE_MM_DEVMEMINTCTXDESTROY 16UL
#define PVR_SRV_BRIDGE_MM_HEAPCFGHEAPCOUNT 30UL
#define PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPCREATE 17UL
#define PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPDESTROY 18UL
enum pvr_srv_error {
   PVR_SRV_OK,
   PVR_SRV_ERROR_RETRY = 25,
   PVR_SRV_ERROR_BRIDGE_CALL_FAILED = 37,
   PVR_SRV_ERROR_FORCE_I32 = 0x7fffffff
};
struct drm_srvkm_cmd {
   uint32_t bridge_id;
   uint32_t bridge_func_id;
   uint64_t in_data_ptr;
   uint64_t out_data_ptr;
   uint32_t in_data_size;
   uint32_t out_data_size;
};
struct pvr_srv_bridge_connect_cmd {
   uint32_t build_options;
   uint32_t DDK_build;
   uint32_t DDK_version;
   uint32_t flags;
} PACKED;
struct pvr_srv_bridge_connect_ret {
   uint64_t bvnc;
   enum pvr_srv_error error;
   uint32_t capability_flags;
   uint8_t kernel_arch;
} PACKED;
struct pvr_srv_bridge_disconnect_ret {
   enum pvr_srv_error error;
} PACKED;
struct pvr_srv_devmem_int_ctx_create_cmd {
   uint32_t kernel_memory_ctx;
} PACKED;
struct pvr_srv_devmem_int_ctx_create_ret {
   void *server_memctx;
   void *server_memctx_data;
   enum pvr_srv_error error;
   uint32_t cpu_cache_line_size;
} PACKED;
struct pvr_srv_heap_count_cmd {
   uint32_t heap_config_index;
} PACKED;
struct pvr_srv_heap_count_ret {
   enum pvr_srv_error error;
   uint32_t heap_count;
} PACKED;
struct pvr_srv_devmem_int_heap_create_cmd {
   pvr_dev_addr_t base_addr;
   uint64_t size;
   void *server_memctx;
   uint32_t log2_page_size;
} PACKED;
struct pvr_srv_devmem_int_heap_create_ret {
   void *server_heap;
   enum pvr_srv_error error;
} PACKED;

#define DRM_IOCTL_SRVKM_CMD DRM_IOWR(DRM_COMMAND_BASE,struct drm_srvkm_cmd)
static int mode,ioctls,version_frees;
static drmVersion version;
drmVersionPtr drmGetVersion(int fd){assert(fd==9);return &version;}
void drmFreeVersion(drmVersionPtr p){assert(p==&version);version_frees++;}
int drmIoctl(int fd,unsigned long request,void *arg);
#include "pvr_srv_ddk19.h"
int drmIoctl(int fd,unsigned long request,void *arg){
 assert(fd==9&&request==DRM_IOCTL_SRVKM_CMD);struct drm_srvkm_cmd *cmd=arg;ioctls++;
 if(mode==1){errno=EIO;return -1;}
 if(cmd->bridge_id==6){
  uint8_t *b=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t e=mode==2?5:mode==3?25:0;
  memset(b,0,cmd->out_data_size);
  if(cmd->bridge_func_id==15){assert(cmd->in_data_size==4&&cmd->out_data_size==24);uint64_t handles[2]={0x111,0x222};uint32_t line=64;memcpy(b,handles,16);memcpy(b+16,&line,4);memcpy(b+20,&e,4);}
  else if(cmd->bridge_func_id==28){assert(cmd->in_data_size==4&&cmd->out_data_size==8);uint32_t count=17;memcpy(b,&count,4);memcpy(b+4,&e,4);}
  else if(cmd->bridge_func_id==17){assert(cmd->in_data_size==28&&cmd->out_data_size==12);uint8_t *i=(void*)(uintptr_t)cmd->in_data_ptr;uint64_t context,base,size;uint32_t page;memcpy(&context,i,8);memcpy(&base,i+8,8);memcpy(&size,i+16,8);memcpy(&page,i+24,4);assert(context==0x111&&base==UINT64_C(0x65c0000000)&&size==0x80000000&&page==12);uint64_t heap=0x333;memcpy(b,&heap,8);memcpy(b+8,&e,4);}
  else{assert((cmd->bridge_func_id==16||cmd->bridge_func_id==18)&&cmd->in_data_size==8&&cmd->out_data_size==4);memcpy(b,&e,4);}
  return 0;
 }
 if(cmd->bridge_func_id==PVR_SRV_BRIDGE_SRVCORE_CONNECT){
  assert(cmd->in_data_size==16&&cmd->out_data_size==17);
  struct mfw19_connect_in *in=(void*)(uintptr_t)cmd->in_data_ptr;
  struct mfw19_connect_out *out=(void*)(uintptr_t)cmd->out_data_ptr;
  assert(in->flags==32&&in->options==0x80000010&&in->version==0x10009&&in->build==4893595);
  *out=(struct mfw19_connect_out){.arch=64,.caps=0x100,.pvr=0x8a847,.rgx=0x1447,.error=mode==2?5:0};
 }else{assert(cmd->bridge_func_id==PVR_SRV_BRIDGE_SRVCORE_DISCONNECT);assert(cmd->in_data_size==4&&cmd->out_data_size==4);*(uint32_t*)(uintptr_t)cmd->out_data_ptr=mode==2?5:0;}
 return 0;
}
int main(void){
 version=(drmVersion){.name="pvr",.version_major=1,.version_minor=9,.version_patchlevel=4893595};
 struct pvr_srv_bridge_connect_cmd in={.flags=32,.build_options=0x80000010,.DDK_version=0x10009,.DDK_build=4893595};
 struct pvr_srv_bridge_connect_ret out={0};
 assert(!pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))&&out.error==0&&out.kernel_arch==64&&out.capability_flags==0x100&&out.bvnc==UINT64_C(0x0016003100150010));
 assert(ioctls==1&&version_frees==1);
 version.version_minor=17;assert(pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))<0&&ioctls==1);version.version_minor=9;
 version.version_patchlevel=1;assert(pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))<0&&ioctls==1);version.version_patchlevel=4893595;
 version.name="other";assert(pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))<0&&ioctls==1);version.name="pvr";
 in.flags=0;assert(pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))<0&&ioctls==1);in.flags=32;
 assert(pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in)-1,&out,sizeof(out))<0&&ioctls==1);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))<0);
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,1,0,&in,sizeof(in),&out,sizeof(out))&&out.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 struct pvr_srv_bridge_disconnect_ret disconnected;
 mode=0;assert(!pvr_srv_ddk19_bridge_call(9,1,1,NULL,0,&disconnected,sizeof(disconnected))&&disconnected.error==0);
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,1,1,NULL,0,&disconnected,sizeof(disconnected))&&disconnected.error!=0);
 int previous=ioctls;assert(pvr_srv_ddk19_bridge_call(9,6,32,&in,sizeof(in),&out,sizeof(out))<0&&errno==ENOTSUP&&ioctls==previous);
 mode=0;
 struct pvr_srv_devmem_int_ctx_create_cmd ci={0};struct pvr_srv_devmem_int_ctx_create_ret co;
 assert(!pvr_srv_ddk19_bridge_call(9,6,15,&ci,sizeof(ci),&co,sizeof(co))&&co.error==0&&co.server_memctx==(void*)0x111&&co.server_memctx_data==(void*)0x222&&co.cpu_cache_line_size==64);
 struct pvr_srv_heap_count_cmd hc={0};struct pvr_srv_heap_count_ret hr;
 assert(!pvr_srv_ddk19_bridge_call(9,6,30,&hc,sizeof(hc),&hr,sizeof(hr))&&hr.error==0&&hr.heap_count==17);
 struct pvr_srv_devmem_int_heap_create_cmd hi={.base_addr={.addr=UINT64_C(0x65c0000000)},.size=0x80000000,.server_memctx=(void*)0x111,.log2_page_size=12};struct pvr_srv_devmem_int_heap_create_ret ho;
 assert(!pvr_srv_ddk19_bridge_call(9,6,17,&hi,sizeof(hi),&ho,sizeof(ho))&&ho.error==0&&ho.server_heap==(void*)0x333);
 previous=ioctls;ci.kernel_memory_ctx=1;assert(pvr_srv_ddk19_bridge_call(9,6,15,&ci,sizeof(ci),&co,sizeof(co))<0&&ioctls==previous);ci.kernel_memory_ctx=0;
 hi.log2_page_size=14;assert(pvr_srv_ddk19_bridge_call(9,6,17,&hi,sizeof(hi),&ho,sizeof(ho))<0&&ioctls==previous);hi.log2_page_size=12;
 assert(pvr_srv_ddk19_bridge_call(9,6,15,NULL,sizeof(ci),&co,sizeof(co))<0&&ioctls==previous);
 uint64_t handle=0x333;enum pvr_srv_error error;
 assert(!pvr_srv_ddk19_bridge_call(9,6,18,&handle,8,&error,4)&&error==0);handle=0x111;
 assert(!pvr_srv_ddk19_bridge_call(9,6,16,&handle,8,&error,4)&&error==0);
 mode=3;assert(!pvr_srv_ddk19_bridge_call(9,6,16,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,6,30,&hc,sizeof(hc),&hr,sizeof(hr))&&hr.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,6,30,&hc,sizeof(hc),&hr,sizeof(hr))<0);mode=0;
 puts("PASS exact Mesa19 adapter: typed CONNECT/DISCONNECT, driver guards, error translation and fail-closed unsupported bridge");
}
