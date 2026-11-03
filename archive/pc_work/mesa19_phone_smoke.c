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
/* Diagnostic exact1.9-only import chain; not a generic modern bridge translation. */
static int legacy_import_call(int fd,uint32_t function,uint64_t handle,void *out,uint32_t size){
 struct drm_srvkm_cmd cmd={.bridge_id=6,.bridge_func_id=function,.in_data_ptr=(uintptr_t)&handle,.out_data_ptr=(uintptr_t)out,.in_data_size=8,.out_data_size=size};
 if(!handle||(function!=3&&function!=4&&function!=6&&function!=7))return -1;
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
}
#include "mfw_kicksync_abi.h"
#include "mfw_kicksync_trial.h"
static int sync_readback(int fd,void *export_handle,uint32_t expected,void *private_data,void *sync_handle){
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
 int kick_rc=firmware_sync_trial(fd,private_data,sync_handle,memory);
 if(kick_rc==2)return 2;
 if(kick_rc)rc=1;
 done:
 if(memory!=MAP_FAILED&&munmap((void*)memory,4096)){rc=2;return rc;}
 if(imported.pmr){error=UINT32_MAX;if(legacy_import_call(fd,7,imported.pmr,&error,4)||error)return 2;}
 if(made.handle){error=UINT32_MAX;if(legacy_import_call(fd,4,made.handle,&error,4)||error)return 2;}
 return rc;
}
int main(void){
 _Static_assert(sizeof(void*)==8,"64-bit ABI only");
 int fd=open("/dev/dri/renderD128",O_RDWR|O_CLOEXEC);if(fd<0){perror("render open");return 1;}
 struct pvr_srv_bridge_connect_cmd in={.flags=32,.build_options=0x80000010,.DDK_version=0x10009,.DDK_build=4893595};
 struct pvr_srv_bridge_connect_ret out={0};
 struct pvr_srv_devmem_int_ctx_create_cmd ci={0};struct pvr_srv_devmem_int_ctx_create_ret co={0};
 struct pvr_srv_heap_count_cmd hi={0};struct pvr_srv_heap_count_ret ho={0};
 struct pvr_srv_devmem_int_heap_create_cmd heap_in={.base_addr={.addr=UINT64_C(0x65c0000000)},.size=0x80000000,.log2_page_size=12};
 struct pvr_srv_devmem_int_heap_create_ret heap_out={0};
 struct pvr_srv_devmem_int_reserve_range_cmd ri={.addr={.addr=UINT64_C(0x65c0000000)},.size=4096};
 struct pvr_srv_devmem_int_reserve_range_ret ro={0};
 uint32_t table=0;const char annotation[]="test";
 struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd ai={.block_size=4096,.size=4096,.mapping_table=&table,.annotation=annotation,.annotation_size=5,.log2_page_size=12,.phy_blocks=1,.virt_blocks=1,.flags=0x80004333};
 struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret ao={0};
 struct pvr_srv_devmem_int_map_pmr_cmd mi={.flags=0x303};struct pvr_srv_devmem_int_map_pmr_ret mo={0};
 volatile uint32_t *memory=MAP_FAILED;
 struct pvr_srv_bridge_alloc_sync_primitive_block_ret sync={0};
 struct pvr_srv_bridge_sync_prim_set_cmd si={.index=0,.value=0x12345678};enum pvr_srv_error sync_error;
 uint8_t reset[16]={0};struct pvr_srv_rgx_create_transfer_context_cmd ti={.reset_framework_cmd=reset,.reset_framework_cmd_size=16};struct pvr_srv_rgx_create_transfer_context_ret to={0};
 int connected=0,rc=1;
 if(pvr_srv_ddk19_bridge_call(fd,1,0,&in,sizeof(in),&out,sizeof(out))||out.error)goto done;
 connected=1;
 if(pvr_srv_ddk19_bridge_call(fd,6,15,&ci,sizeof(ci),&co,sizeof(co))||co.error||!co.server_memctx)goto done;
 if(pvr_srv_ddk19_bridge_call(fd,6,30,&hi,sizeof(hi),&ho,sizeof(ho))||ho.error||!ho.heap_count||ho.heap_count>64)goto done;
 heap_in.server_memctx=co.server_memctx;
 if(pvr_srv_ddk19_bridge_call(fd,6,17,&heap_in,sizeof(heap_in),&heap_out,sizeof(heap_out))||heap_out.error||!heap_out.server_heap)goto done;
 ri.server_heap=heap_out.server_heap;
 if(pvr_srv_ddk19_bridge_call(fd,6,21,&ri,sizeof(ri),&ro,sizeof(ro))||ro.error||!ro.reservation)goto done;
 ai.pid=(uint32_t)getpid();
 if(pvr_srv_ddk19_bridge_call(fd,6,10,&ai,sizeof(ai),&ao,sizeof(ao))||ao.error||!ao.pmr)goto done;
 if((uintptr_t)ao.pmr>INT64_MAX/4096)goto done;
 memory=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)((uintptr_t)ao.pmr*4096));
 if(memory==MAP_FAILED)goto done;
 for(unsigned i=0;i<1024;i++)if(memory[i])goto done;
 for(unsigned i=0;i<1024;i++)memory[i]=0x4d465700u^(i*0x01010101u);
 __sync_synchronize();
 for(unsigned i=0;i<1024;i++)if(memory[i]!=(0x4d465700u^(i*0x01010101u)))goto done;
 mi.server_heap=heap_out.server_heap;mi.reservation=ro.reservation;mi.pmr=ao.pmr;
 if(pvr_srv_ddk19_bridge_call(fd,6,19,&mi,sizeof(mi),&mo,sizeof(mo))||mo.error||!mo.mapping)goto done;
 if(pvr_srv_ddk19_bridge_call(fd,2,0,NULL,0,&sync,sizeof(sync))||sync.error||!sync.handle||!sync.pmr||sync.size<4)goto done;
 si.handle=sync.handle;
 if(pvr_srv_ddk19_bridge_call(fd,2,2,&si,sizeof(si),&sync_error,4)||sync_error)goto done;
 int readback_rc=sync_readback(fd,sync.pmr,si.value,co.server_memctx_data,sync.handle);
 if(readback_rc==2)goto disconnect;
 if(readback_rc)goto done;
 si.value=0;
 if(pvr_srv_ddk19_bridge_call(fd,2,2,&si,sizeof(si),&sync_error,4)||sync_error)goto done;
 printf("MESA19_SYNC_ALLOC_SET_PASS size=%u addr=%x; no shader completion tested\n",sync.size,sync.addr);
 ti.priv_data=co.server_memctx_data;
 if(pvr_srv_ddk19_bridge_call(fd,128,0,&ti,sizeof(ti),&to,sizeof(to))||to.error||!to.transfer_context){fprintf(stderr,"transfer create error=%u\n",to.error);goto done;}
 printf("MESA19_TRANSFER_CONTEXT_CREATE_PASS reset-framework disabled; no submit\n");
 printf("MESA19_NATIVE_PMR_GPU_MAP_PASS bytes=4096; context_PASS heaps=%u cache_line=%u; no rendering job\n",ho.heap_count,co.cpu_cache_line_size);
 rc=0;
done:
 if(to.transfer_context){int freed=0;for(unsigned i=0;i<50;i++){enum pvr_srv_error e=PVR_SRV_ERROR_BRIDGE_CALL_FAILED;
  if(pvr_srv_ddk19_bridge_call(fd,128,1,&to.transfer_context,8,&e,4))break;
  if(!e){freed=1;break;}if(e!=PVR_SRV_ERROR_RETRY)break;
  struct timespec pause={.tv_nsec=10000000};nanosleep(&pause,NULL);}
  if(!freed){rc=1;goto disconnect;}puts("MESA19_TRANSFER_CONTEXT_FREE_PASS");}
 if(sync.handle){enum pvr_srv_error e=PVR_SRV_ERROR_BRIDGE_CALL_FAILED;
  if(pvr_srv_ddk19_bridge_call(fd,2,1,&sync.handle,8,&e,4)||e){rc=1;goto disconnect;}}
 if(memory!=MAP_FAILED&&munmap((void*)memory,4096))rc=1;
 if(mo.mapping&&drop_handle(fd,20,mo.mapping)){rc=1;goto disconnect;}
 if(ro.reservation&&drop_handle(fd,22,ro.reservation)){rc=1;goto disconnect;}
 if(ao.pmr&&drop_handle(fd,8,ao.pmr)){rc=1;goto disconnect;}
 if(heap_out.server_heap&&drop_handle(fd,18,heap_out.server_heap)){rc=1;goto disconnect;}
 if(co.server_memctx&&drop_handle(fd,16,co.server_memctx))rc=1;
disconnect:
 if(connected){struct pvr_srv_bridge_disconnect_ret d;if(pvr_srv_ddk19_bridge_call(fd,1,1,NULL,0,&d,sizeof(d))||d.error)rc=1;}
 close(fd);printf("MESA19_TRIAL_RC=%d\n",rc);return rc;
}
