#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#define DRM_COMMAND_BASE 0x40
#define DRM_IOWR(n,t) _IOWR('d',n,t)
typedef struct {int version_major,version_minor,version_patchlevel;char *name;} drmVersion,*drmVersionPtr;
int drmIoctl(int,unsigned long,void*);
drmVersionPtr drmGetVersion(int);
void drmFreeVersion(drmVersionPtr);

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

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
struct pvr_srv_devmem_int_reserve_range_cmd {
   pvr_dev_addr_t addr;
   uint64_t size;
   void *server_heap;
} PACKED;
struct pvr_srv_devmem_int_reserve_range_ret {
   void *reservation;
   enum pvr_srv_error error;
} PACKED;
struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd {
   uint64_t block_size;
   uint64_t size;
   uint32_t *mapping_table;
   const char *annotation;
   uint32_t annotation_size;
   uint32_t log2_page_size;
   uint32_t phy_blocks;
   uint32_t virt_blocks;
   uint32_t pdump_flags;
   uint32_t pid;
   uint64_t flags;
} PACKED;
struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret {
   void *pmr;
   enum pvr_srv_error error;
   uint64_t out_flags;
} PACKED;
struct pvr_srv_devmem_int_map_pmr_cmd {
   void *server_heap;
   void *pmr;
   void *reservation;
   uint64_t flags;
} PACKED;
struct pvr_srv_devmem_int_map_pmr_ret {
   void *mapping;
   enum pvr_srv_error error;
} PACKED;
struct pvr_srv_bridge_alloc_sync_primitive_block_ret {
   void *handle;
   void *pmr;
   enum pvr_srv_error error;
   uint32_t size;
   uint32_t addr;
} PACKED;
struct pvr_srv_bridge_sync_prim_set_cmd {
   void *handle;
   uint32_t index;
   uint32_t value;
} PACKED;
struct pvr_srv_rgx_create_transfer_context_cmd {
   uint64_t robustness_address;
   void *priv_data;
   uint8_t *reset_framework_cmd;
   uint32_t context_flags;
   uint32_t reset_framework_cmd_size;
   uint32_t packed_ccb_size_u8888;
   uint32_t priority;
} PACKED;
struct pvr_srv_rgx_create_transfer_context_ret {
   void *cli_pmr_mem;
   void *transfer_context;
   void *usc_pmr_mem;
   enum pvr_srv_error error;
} PACKED;
#include "pvr_srv_ddk19.h"
/* Standalone smoke wrapper for the real DRM UAPI, not libdrm mock IO. */
struct raw_drm_version {int major,minor,patch;uint64_t name_len;char *name;uint64_t date_len;char *date;uint64_t desc_len;char *desc;};
_Static_assert(sizeof(struct raw_drm_version)==64,"arm64 DRM version UAPI");
int drmIoctl(int fd,unsigned long request,void *arg){int rc;do{rc=ioctl(fd,request,arg);}while(rc<0&&errno==EINTR);return rc;}
drmVersionPtr drmGetVersion(int fd){
 static char name[64];static drmVersion result;
 memset(name,0,sizeof(name));struct raw_drm_version v={.name_len=63,.name=name};
 if(drmIoctl(fd,DRM_IOWR(0,struct raw_drm_version),&v))return NULL;
 result=(drmVersion){.version_major=v.major,.version_minor=v.minor,.version_patchlevel=v.patch,.name=name};return &result;
}
void drmFreeVersion(drmVersionPtr v){(void)v;}
static int drop_handle(int fd,unsigned command,void *handle){
 for(unsigned i=0;i<20;i++){
  enum pvr_srv_error error=PVR_SRV_ERROR_BRIDGE_CALL_FAILED;
  if(pvr_srv_ddk19_bridge_call(fd,6,command,&handle,8,&error,4))return 1;
  if(error==PVR_SRV_OK)return 0;
  if(error!=PVR_SRV_ERROR_RETRY)return 1;
  struct timespec t={.tv_nsec=10000000};nanosleep(&t,NULL);
 }
 return 1;
}

/* Exact legacy generated CREATE/DESTROY layouts; no KICK function provided. */
struct compute_create_in {uint32_t priority,framework_bytes;uint8_t *framework;void *private_data;uint64_t resume_signal;} PACKED;
struct compute_create_out {void *context;uint32_t error;} PACKED;
_Static_assert(sizeof(struct compute_create_in)==32,"legacy compute input");
_Static_assert(sizeof(struct compute_create_out)==12,"legacy compute output");
static int compute_call(int fd,unsigned fn,void *in,unsigned insize,void *out,unsigned outsize){
 if(fn>1)return -1;
 struct drm_srvkm_cmd cmd={.bridge_id=129,.bridge_func_id=fn,.in_data_ptr=(uintptr_t)in,.out_data_ptr=(uintptr_t)out,.in_data_size=insize,.out_data_size=outsize};
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
}
int main(void){
 int fd=open("/dev/dri/renderD128",O_RDWR|O_CLOEXEC);if(fd<0){perror("render open");return 1;}
 struct pvr_srv_bridge_connect_cmd in={.flags=32,.build_options=0x80000010,.DDK_version=0x10009,.DDK_build=4893595};
 struct pvr_srv_bridge_connect_ret out={0};
 struct pvr_srv_devmem_int_ctx_create_cmd ci={0};struct pvr_srv_devmem_int_ctx_create_ret co={0};
 uint8_t reset[16]={0};struct compute_create_in c={.framework_bytes=16,.framework=reset};struct compute_create_out result={.error=UINT32_MAX};
 int connected=0,rc=1;
 if(pvr_srv_ddk19_bridge_call(fd,1,0,&in,sizeof(in),&out,sizeof(out))||out.error)goto done;
 connected=1;
 if(pvr_srv_ddk19_bridge_call(fd,6,15,&ci,sizeof(ci),&co,sizeof(co))||co.error||!co.server_memctx||!co.server_memctx_data)goto done;
 c.private_data=co.server_memctx_data;
 if(compute_call(fd,0,&c,sizeof(c),&result,sizeof(result))||result.error||!result.context){fprintf(stderr,"COMPUTE_CREATE_FAIL error=%u\n",result.error);goto done;}
 puts("COMPUTE_CREATE_PASS; framework disabled, no shader/register payload submitted");
 rc=0;
done:
 if(result.context){int freed=0;for(unsigned i=0;i<50;i++){
 uint32_t error=UINT32_MAX;
 if(compute_call(fd,1,&result.context,8,&error,4))break;
 if(!error){freed=1;break;}if(error!=25)break;
 struct timespec pause={.tv_nsec=10000000};nanosleep(&pause,NULL);
 }if(!freed){rc=1;goto disconnect;}puts("COMPUTE_DESTROY_PASS");}
 if(co.server_memctx&&drop_handle(fd,16,co.server_memctx))rc=1;
disconnect:
 if(connected){struct pvr_srv_bridge_disconnect_ret d;if(pvr_srv_ddk19_bridge_call(fd,1,1,NULL,0,&d,sizeof(d))||d.error)rc=1;}
 close(fd);printf("COMPUTE_LIFECYCLE_RC=%d\n",rc);return rc;
}
