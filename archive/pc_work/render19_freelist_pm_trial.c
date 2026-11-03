#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stddef.h>
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
#include "pvr_srv_ddk19_pm.h"
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


struct freelist_create_in {uint32_t max_pages,init_pages,grow_pages,threshold;void *global_list;uint32_t check;uint64_t address;void *pmr;uint64_t offset;} PACKED;
_Static_assert(sizeof(struct freelist_create_in)==52,"legacy freelist ABI");
struct render_create_in {uint32_t priority;uint64_t vdm_stack;uint32_t framework_size;uint8_t *framework;void *private_data;} PACKED;
struct render_create_out {void *context;uint32_t error;} PACKED;
_Static_assert(sizeof(struct render_create_in)==32&&offsetof(struct render_create_in,vdm_stack)==4&&sizeof(struct render_create_out)==12,"exact legacy render ABI");
static int render_call(int fd,unsigned fn,void *in,unsigned is,void *out,unsigned os){
 if(fn!=8&&fn!=9&&fn!=12&&fn!=13)return -1;
 struct drm_srvkm_cmd c={.bridge_id=130,.bridge_func_id=fn,.in_data_ptr=(uintptr_t)in,.out_data_ptr=(uintptr_t)out,.in_data_size=is,.out_data_size=os};return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&c);
}

struct heap_details_in {uint32_t config,index,name_bytes;char *name;} PACKED;
struct heap_details_out {char *name;uint64_t base,bytes;uint32_t page,align,tiling,error;} PACKED;
_Static_assert(sizeof(struct heap_details_in)==20,"legacy heap detail input");
_Static_assert(sizeof(struct heap_details_out)==40,"legacy heap detail output");
struct arena {char name[128];uint64_t base,bytes;uint32_t page;void *handle;};
struct page {void *reservation,*pmr,*mapping;volatile uint32_t *cpu;uint64_t va;unsigned arena;};
static int query_heap(int fd,unsigned index,char *name,struct heap_details_out *out){
 struct heap_details_in in={.index=index,.name_bytes=128,.name=name};
 struct drm_srvkm_cmd c={.bridge_id=6,.bridge_func_id=30,.in_data_ptr=(uintptr_t)&in,.out_data_ptr=(uintptr_t)out,.in_data_size=sizeof(in),.out_data_size=sizeof(*out)};
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&c);
}
int main(void){
 int fd=open("/dev/dri/renderD128",O_RDWR|O_CLOEXEC);if(fd<0){perror("render");return 1;}
 struct pvr_srv_bridge_connect_cmd in={.flags=32,.build_options=0x80000010,.DDK_version=0x10009,.DDK_build=4893595};struct pvr_srv_bridge_connect_ret out={0};
 struct pvr_srv_devmem_int_ctx_create_cmd ci={0};struct pvr_srv_devmem_int_ctx_create_ret co={0};
 struct pvr_srv_heap_count_cmd hi={0};struct pvr_srv_heap_count_ret ho={0};
 struct arena arenas[3]={0};struct page pages[11]={0};for(unsigned i=0;i<11;i++)pages[i].cpu=MAP_FAILED;
 struct render_create_out render={0},freelist={0};
 int connected=0,rc=1;unsigned checked=0;
 if(pvr_srv_ddk19_bridge_call(fd,1,0,&in,sizeof(in),&out,sizeof(out))||out.error)goto done;
 connected=1;
 if(pvr_srv_ddk19_bridge_call(fd,6,15,&ci,sizeof(ci),&co,sizeof(co))||co.error||!co.server_memctx)goto done;
 if(pvr_srv_ddk19_bridge_call(fd,6,30,&hi,sizeof(hi),&ho,sizeof(ho))||ho.error||ho.heap_count>64)goto done;
 for(unsigned i=0;i<ho.heap_count;i++){
 char name[128]={0};struct heap_details_out d={.error=UINT32_MAX};if(query_heap(fd,i,name,&d)||d.error)goto done;name[127]=0;
 int slot=!strcmp(name,"General")?0:!strcmp(name,"PDS Code and Data")?1:!strcmp(name,"USC Code")?2:-1;
 if(slot<0)continue;
 if(arenas[slot].bytes||d.page!=12||d.bytes<32768||d.base>UINT64_C(0xffffffffff)-d.bytes)goto done;
 snprintf(arenas[slot].name,128,"%s",name);arenas[slot].base=d.base;arenas[slot].bytes=d.bytes;arenas[slot].page=d.page;
 }
 for(unsigned i=0;i<3;i++){
 if(!arenas[i].bytes)goto done;
 struct pvr_srv_devmem_int_heap_create_cmd hc={.base_addr={.addr=arenas[i].base},.size=arenas[i].bytes,.server_memctx=co.server_memctx,.log2_page_size=12};struct pvr_srv_devmem_int_heap_create_ret hr={0};
 if(pvr_srv_ddk19_bridge_call(fd,6,17,&hc,sizeof(hc),&hr,sizeof(hr))||hr.error||!hr.server_heap)goto done;
 arenas[i].handle=hr.server_heap;
 printf("HEAP %s base=%llx bytes=%llx\n",arenas[i].name,(unsigned long long)arenas[i].base,(unsigned long long)arenas[i].bytes);
 }
 for(unsigned i=0;i<11;i++){
 unsigned a=i<8||i==10?0:i==8?1:2;pages[i].arena=a;pages[i].va=arenas[a].base+(i<8||i==10?i*4096:65536);
 if(pages[i].va<arenas[a].base||pages[i].va-arenas[a].base>arenas[a].bytes-4096)goto done;
 struct pvr_srv_devmem_int_reserve_range_cmd ri={.addr={.addr=pages[i].va},.size=4096,.server_heap=arenas[a].handle};struct pvr_srv_devmem_int_reserve_range_ret ro={0};
 if(pvr_srv_ddk19_bridge_call(fd,6,21,&ri,sizeof(ri),&ro,sizeof(ro))||ro.error||!ro.reservation)goto done;
 pages[i].reservation=ro.reservation;
 uint32_t table=0;const char note[]="test";struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd ai={.block_size=4096,.size=4096,.mapping_table=&table,.annotation=note,.annotation_size=5,.log2_page_size=12,.phy_blocks=1,.virt_blocks=1,.pid=(uint32_t)getpid(),.flags=0x80004333};struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret ao={0};
 if(i==10){ai.flags=(ai.flags&~UINT64_C(0x30))|UINT64_C(0x04000000);uint32_t old_flags=0;if(!mfw19_memory_flags(ai.flags,false,&old_flags)||(old_flags&0x30))goto done;}
 if(pvr_srv_ddk19_bridge_call(fd,6,10,&ai,sizeof(ai),&ao,sizeof(ao))||ao.error||!ao.pmr)goto done;
 pages[i].pmr=ao.pmr;
 if(i!=10){
 if((uintptr_t)ao.pmr>INT64_MAX/4096)goto done;
 pages[i].cpu=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)((uintptr_t)ao.pmr*4096));if(pages[i].cpu==MAP_FAILED)goto done;
 for(unsigned w=0;w<1024;w++)if(pages[i].cpu[w])goto done;
 for(unsigned w=0;w<1024;w++)pages[i].cpu[w]=0x4d465700u^(i<<20)^w;
 __sync_synchronize();for(unsigned w=0;w<1024;w++)if(pages[i].cpu[w]!=(0x4d465700u^(i<<20)^w))goto done;
 }
 struct pvr_srv_devmem_int_map_pmr_cmd mi={.server_heap=arenas[a].handle,.pmr=pages[i].pmr,.reservation=pages[i].reservation,.flags=0x303};struct pvr_srv_devmem_int_map_pmr_ret mo={0};if(i==10)mi.flags|=UINT64_C(0x04000000);
 if(pvr_srv_ddk19_bridge_call(fd,6,19,&mi,sizeof(mi),&mo,sizeof(mo))||mo.error||!mo.mapping)goto done;
 pages[i].mapping=mo.mapping;checked++;
 }
 
 uint8_t framework[16]={0};struct render_create_in ri={.vdm_stack=pages[2].va,.framework_size=16,.framework=framework,.private_data=co.server_memctx_data};
 if(render_call(fd,12,&ri,sizeof(ri),&render,sizeof(render))||render.error||!render.context){fprintf(stderr,"RENDER_CREATE_FAIL error=%u\n",render.error);goto done;}
 
 puts("RENDER_CONTEXT_CREATE_PASS stack4Kowned frameworkdisabled; no TA/3D submitted");
 struct freelist_create_in fi={.max_pages=32,.init_pages=32,.address=pages[10].va,.pmr=pages[10].pmr};
 if(render_call(fd,8,&fi,sizeof(fi),&freelist,sizeof(freelist))||freelist.error||!freelist.context){fprintf(stderr,"FREELIST_CREATE_FAIL error=%u\n",freelist.error);goto done;}
 puts("FREELIST_PM_PROTECT_CREATE_PASS pages32 GPUonlytable; no TA/3D submitted");rc=0;
done:
 if(freelist.context){int freed=0;for(unsigned i=0;i<50;i++){uint32_t e=UINT32_MAX;if(render_call(fd,9,&freelist.context,8,&e,4))break;if(!e){freed=1;break;}if(e!=25)break;struct timespec t={.tv_nsec=10000000};nanosleep(&t,NULL);}if(!freed){rc=1;goto disconnect;}puts("FREELIST_DESTROY_PASS");}
 if(render.context){int freed=0;for(unsigned i=0;i<50;i++){uint32_t e=UINT32_MAX;if(render_call(fd,13,&render.context,8,&e,4))break;if(!e){freed=1;break;}if(e!=25)break;struct timespec t={.tv_nsec=10000000};nanosleep(&t,NULL);}if(!freed){rc=1;goto disconnect;}puts("RENDER_CONTEXT_DESTROY_PASS");}
 for(int i=10;i>=0;i--){
 if(pages[i].cpu!=MAP_FAILED&&munmap((void*)pages[i].cpu,4096)){rc=1;goto disconnect;}
 if(pages[i].mapping&&drop_handle(fd,20,pages[i].mapping)){rc=1;goto disconnect;}
 if(pages[i].reservation&&drop_handle(fd,22,pages[i].reservation)){rc=1;goto disconnect;}
 if(pages[i].pmr&&drop_handle(fd,8,pages[i].pmr)){rc=1;goto disconnect;}
 }
 for(int i=2;i>=0;i--)if(arenas[i].handle&&drop_handle(fd,18,arenas[i].handle)){rc=1;goto disconnect;}
 if(co.server_memctx&&drop_handle(fd,16,co.server_memctx))rc=1;
disconnect:
 if(connected){struct pvr_srv_bridge_disconnect_ret d;if(pvr_srv_ddk19_bridge_call(fd,1,1,NULL,0,&d,sizeof(d))||d.error)rc=1;}
 close(fd);printf("FREELIST_PM_PROTECT_RC=%d pages=%u\n",rc,checked);return rc;
}
