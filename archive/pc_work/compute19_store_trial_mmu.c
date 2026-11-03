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


#include "mfw_cdm19_fw_wire.h"
struct cmp_create_in {uint32_t priority,framework_size;uint8_t *framework;void *private_data;uint64_t resume;} PACKED;
struct cmp_create_out {void *context;uint32_t error;} PACKED;
struct cmp_kick_in {
 void *context;uint32_t cache_seq,fence_count;void **fence_blocks;uint32_t *fence_offsets,*fence_values;
 uint32_t update_count;void **update_blocks;uint32_t *update_offsets,*update_values;
 uint32_t server_count;uint32_t *server_flags;void **server_syncs;
 int32_t check_fence,update_timeline;char *fence_name;uint32_t command_bytes;uint8_t *command;
 uint32_t pdump_flags,job_ref;uint64_t robustness_addr;
} PACKED;
struct cmp_kick_out {int32_t fence;uint32_t error;} PACKED;
_Static_assert(sizeof(struct cmp_create_in)==32&&sizeof(struct cmp_create_out)==12,"legacy create");
_Static_assert(sizeof(struct cmp_kick_in)==132&&offsetof(struct cmp_kick_in,command_bytes)==104&&offsetof(struct cmp_kick_in,command)==108,"legacy kick");
static struct mfw_cdm19_fw_cmd trial_packet;
static volatile uint32_t *trial_output;
static int cmp_call(int fd,unsigned fn,void *in,unsigned is,void *out,unsigned os){
 if(fn>2)return -1;
 struct drm_srvkm_cmd c={.bridge_id=129,.bridge_func_id=fn,.in_data_ptr=(uintptr_t)in,.out_data_ptr=(uintptr_t)out,.in_data_size=is,.out_data_size=os};return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&c);
}
static int compute_shader_trial(int fd,void *private_data,void *sync_handle,volatile uint32_t *sync_memory){
 uint8_t framework[16]={0};struct cmp_create_in ci={.framework_size=16,.framework=framework,.private_data=private_data};struct cmp_create_out co={.error=UINT32_MAX};
 if(!private_data||!sync_handle||!trial_output||sync_memory[0]!=0x12345678||trial_output[0]!=0xdeadbeef)return 1;
 if(cmp_call(fd,0,&ci,sizeof(ci),&co,sizeof(co))||co.error||!co.context){fprintf(stderr,"COMPUTE_CREATE_FAIL %u\n",co.error);return 1;}
 void *block=sync_handle;uint32_t offset=0,value=0x87654321;char name[32]="MICROFW bounded USC store";
 struct cmp_kick_in ki={.context=co.context,.update_count=1,.update_blocks=&block,.update_offsets=&offset,.update_values=&value,.check_fence=-1,.update_timeline=-1,.fence_name=name,.command_bytes=sizeof(trial_packet),.command=(uint8_t*)&trial_packet};struct cmp_kick_out ko={.fence=-1,.error=UINT32_MAX};
 int rc=1;
 __sync_synchronize();
 if(cmp_call(fd,2,&ki,sizeof(ki),&ko,sizeof(ko))||ko.error){fprintf(stderr,"COMPUTE_KICK_FAIL error=%u\n",ko.error);goto cleanup;}
 puts("COMPUTE_KICK_ACCEPTED; awaiting firmware UFO and actual shader store");fflush(stdout);
 for(unsigned i=0;i<500;i++){
  __sync_synchronize();
  if(sync_memory[0]==value){
   if(trial_output[0]!=0x12345678){fprintf(stderr,"COMPUTE_UFO_DONE_BUT_STORE_FAIL %08x\n",trial_output[0]);break;}
   unsigned corrupt=0;for(unsigned w=1;w<1024;w++)if(trial_output[w]!=0xdeadbeef)corrupt++;
   if(corrupt){fprintf(stderr,"COMPUTE_OUTPUT_GUARD_FAIL words=%u\n",corrupt);break;}
   printf("USC_STORE_READBACK_PASS output=%08x UFO=%08x guard_words1023PASS\n",trial_output[0],sync_memory[0]);rc=0;break;
  }
  struct timespec t={.tv_nsec=10000000};nanosleep(&t,NULL);
 }
 if(sync_memory[0]!=value){fprintf(stderr,"COMPUTE_TIMEOUT_RETAIN_RESOURCES output=%08x UFO=%08x\n",trial_output[0],sync_memory[0]);return 2;}
cleanup:
 if(ko.fence>=0)close(ko.fence);
 for(unsigned i=0;i<50;i++){uint32_t error=UINT32_MAX;if(cmp_call(fd,1,&co.context,8,&error,4))return 2;if(!error)return rc;if(error!=25)return 2;struct timespec t={.tv_nsec=10000000};nanosleep(&t,NULL);}return 2;
}
static int load_exact(const char *path,void *dest,size_t bytes){FILE *f=fopen(path,"rb");if(!f)return 1;int bad=fread(dest,1,bytes,f)!=bytes||fgetc(f)!=EOF;return fclose(f)||bad;}

static int legacy_import_call(int fd,uint32_t function,uint64_t handle,void *out,uint32_t size){
 struct drm_srvkm_cmd cmd={.bridge_id=6,.bridge_func_id=function,.in_data_ptr=(uintptr_t)&handle,.out_data_ptr=(uintptr_t)out,.in_data_size=8,.out_data_size=size};
 if(!handle||(function!=3&&function!=4&&function!=6&&function!=7))return -1;
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
}

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
 int kick_rc=compute_shader_trial(fd,private_data,sync_handle,memory);
 if(kick_rc==2)return 2;
 if(kick_rc)rc=1;
 done:
 if(memory!=MAP_FAILED&&munmap((void*)memory,4096)){rc=2;return rc;}
 if(imported.pmr){error=UINT32_MAX;if(legacy_import_call(fd,7,imported.pmr,&error,4)||error)return 2;}
 if(made.handle){error=UINT32_MAX;if(legacy_import_call(fd,4,made.handle,&error,4)||error)return 2;}
 return rc;
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
int main(int argc,char **argv){
 int submit=argc==2&&!strcmp(argv[1],"--submit");if(argc>2||(argc==2&&!submit))return 2;
 int fd=open("/dev/dri/renderD128",O_RDWR|O_CLOEXEC);if(fd<0){perror("render");return 1;}
 struct pvr_srv_bridge_connect_cmd in={.flags=32,.build_options=0x80000010,.DDK_version=0x10009,.DDK_build=4893595};struct pvr_srv_bridge_connect_ret out={0};
 struct pvr_srv_devmem_int_ctx_create_cmd ci={0};struct pvr_srv_devmem_int_ctx_create_ret co={0};
 struct pvr_srv_heap_count_cmd hi={0};struct pvr_srv_heap_count_ret ho={0};
 struct arena arenas[5]={0};struct page pages[12]={0};for(unsigned i=0;i<12;i++)pages[i].cpu=MAP_FAILED;
 struct pvr_srv_bridge_alloc_sync_primitive_block_ret sync={0};
 int connected=0,rc=1;unsigned checked=0;
 if(pvr_srv_ddk19_bridge_call(fd,1,0,&in,sizeof(in),&out,sizeof(out))||out.error)goto done;
 connected=1;
 if(pvr_srv_ddk19_bridge_call(fd,6,15,&ci,sizeof(ci),&co,sizeof(co))||co.error||!co.server_memctx)goto done;
 if(pvr_srv_ddk19_bridge_call(fd,6,30,&hi,sizeof(hi),&ho,sizeof(ho))||ho.error||ho.heap_count>64)goto done;
 for(unsigned i=0;i<ho.heap_count;i++){
 char name[128]={0};struct heap_details_out d={.error=UINT32_MAX};if(query_heap(fd,i,name,&d)||d.error)goto done;name[127]=0;
 int slot=!strcmp(name,"General")?0:!strcmp(name,"PDS Code and Data")?1:!strcmp(name,"USC Code")?2:!strcmp(name,"MMU INIA BRN65273")?3:!strcmp(name,"MMU INIB BRN65273")?4:-1;
 if(slot<0)continue;
 if(arenas[slot].bytes||d.page!=12||d.bytes<32768||d.base>UINT64_C(0xffffffffff)-d.bytes)goto done;
 snprintf(arenas[slot].name,128,"%s",name);arenas[slot].base=d.base;arenas[slot].bytes=d.bytes;arenas[slot].page=d.page;
 }
 for(unsigned i=0;i<5;i++){
 if(!arenas[i].bytes)goto done;
 struct pvr_srv_devmem_int_heap_create_cmd hc={.base_addr={.addr=arenas[i].base},.size=arenas[i].bytes,.server_memctx=co.server_memctx,.log2_page_size=12};struct pvr_srv_devmem_int_heap_create_ret hr={0};
 if(pvr_srv_ddk19_bridge_call(fd,6,17,&hc,sizeof(hc),&hr,sizeof(hr))||hr.error||!hr.server_heap)goto done;
 arenas[i].handle=hr.server_heap;
 printf("HEAP %s base=%llx bytes=%llx\n",arenas[i].name,(unsigned long long)arenas[i].base,(unsigned long long)arenas[i].bytes);
 }
 for(unsigned i=0;i<12;i++){
 unsigned a=i<8?0:i==8?1:i==9?2:i==10?3:4;pages[i].arena=a;pages[i].va=arenas[a].base+(i<8?i*4096:i<10?65536:0);
 if(pages[i].va<arenas[a].base||pages[i].va-arenas[a].base>arenas[a].bytes-4096)goto done;
 struct pvr_srv_devmem_int_reserve_range_cmd ri={.addr={.addr=pages[i].va},.size=4096,.server_heap=arenas[a].handle};struct pvr_srv_devmem_int_reserve_range_ret ro={0};
 if(pvr_srv_ddk19_bridge_call(fd,6,21,&ri,sizeof(ri),&ro,sizeof(ro))||ro.error||!ro.reservation)goto done;
 pages[i].reservation=ro.reservation;
 uint32_t table=0;const char note[]="test";struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd ai={.block_size=4096,.size=4096,.mapping_table=&table,.annotation=note,.annotation_size=5,.log2_page_size=12,.phy_blocks=1,.virt_blocks=1,.pid=(uint32_t)getpid(),.flags=0x80004333};struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret ao={0};
 if(pvr_srv_ddk19_bridge_call(fd,6,10,&ai,sizeof(ai),&ao,sizeof(ao))||ao.error||!ao.pmr)goto done;
 pages[i].pmr=ao.pmr;
 if((uintptr_t)ao.pmr>INT64_MAX/4096)goto done;
 pages[i].cpu=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)((uintptr_t)ao.pmr*4096));if(pages[i].cpu==MAP_FAILED)goto done;
 for(unsigned w=0;w<1024;w++)if(pages[i].cpu[w])goto done;
 for(unsigned w=0;w<1024;w++)pages[i].cpu[w]=0x4d465700u^(i<<20)^w;
 __sync_synchronize();for(unsigned w=0;w<1024;w++)if(pages[i].cpu[w]!=(0x4d465700u^(i<<20)^w))goto done;
 struct pvr_srv_devmem_int_map_pmr_cmd mi={.server_heap=arenas[a].handle,.pmr=pages[i].pmr,.reservation=pages[i].reservation,.flags=0x303};struct pvr_srv_devmem_int_map_pmr_ret mo={0};
 if(pvr_srv_ddk19_bridge_call(fd,6,19,&mi,sizeof(mi),&mo,sizeof(mo))||mo.error||!mo.mapping)goto done;
 pages[i].mapping=mo.mapping;checked++;
 }
 
 if(arenas[0].base!=UINT64_C(0x65c0000000)||arenas[1].base!=UINT64_C(0xa800000000)||arenas[2].base!=UINT64_C(0xba00000000)||arenas[3].base!=UINT64_C(0xf800000000)||arenas[4].base!=UINT64_C(0xf900000000))goto done;
 for(unsigned i=0;i<12;i++)for(unsigned w=0;w<1024;w++)pages[i].cpu[w]=i==0?0xdeadbeef:0;
 if(load_exact("/tmp/compute-pds-candidate.bin",(void*)pages[8].cpu,4096)||load_exact("/tmp/compute-usc-candidate.bin",(void*)pages[9].cpu,4096)||load_exact("/tmp/compute-stream-candidate.bin",(void*)pages[1].cpu,60))goto done;
 trial_output=pages[0].cpu;
 trial_packet=(struct mfw_cdm19_fw_cmd){
 .sCDMRegs={.uCDMReg_CDM_CTRL_STREAM_BASE=pages[1].va,.uCDMReg_COMPUTE_THREAD_BARRIER=1,.uCDMReg_PDS_CTRL=0x44000000,.uCDMReg_TPU=0x20},
 .sCDMRegsCSwitch={.uCDMReg_CDM_CONTEXT_STATE_BASE_ADDR=pages[2].va,
 .uCDMReg_CDM_CONTEXT_PDS0=(UINT64_C(0x10100)<<32)|0x10140,.uCDMReg_CDM_CONTEXT_PDS1=0x28800009,
 .uCDMReg_CDM_TERMINATE_PDS=(UINT64_C(0x10300)<<32)|0x10310,.uCDMReg_CDM_TERMINATE_PDS1=0x28100003,
 .uCDMReg_CDM_RESUME_PDS0=(UINT64_C(0x10200)<<32)|0x10240,.uCDMReg_CDM_RESUME_PDS1=0x00900008},.ui32MaxDeadlineMS=1000};
 __sync_synchronize();
 puts("COMPUTE_TRIAL_STAGED_PASS pages12 heaps5 MMU_INIA_INIB_mapped packet120 stream60; no kick unless --submit");fflush(stdout);
 if(submit){
 if(pvr_srv_ddk19_bridge_call(fd,2,0,NULL,0,&sync,sizeof(sync))||sync.error||!sync.handle||!sync.pmr||sync.size<4)goto done;
 struct pvr_srv_bridge_sync_prim_set_cmd si={.handle=sync.handle,.value=0x12345678};enum pvr_srv_error e;
 if(pvr_srv_ddk19_bridge_call(fd,2,2,&si,sizeof(si),&e,4)||e)goto done;
 int result=sync_readback(fd,sync.pmr,si.value,co.server_memctx_data,sync.handle);
 if(result==2){fprintf(stderr,"RETAINING_ALL_RESOURCES_FD_ALIVE; no automatic further tests\n");fflush(stdout);fflush(stderr);for(;;){struct timespec t={.tv_sec=60};nanosleep(&t,NULL);}}
 if(result)goto done;
 }
 rc=0;
done:
 if(sync.handle){enum pvr_srv_error e;if(pvr_srv_ddk19_bridge_call(fd,2,1,&sync.handle,8,&e,4)||e){rc=1;goto disconnect;}}
 for(int i=11;i>=0;i--){
 if(pages[i].cpu!=MAP_FAILED&&munmap((void*)pages[i].cpu,4096)){rc=1;goto disconnect;}
 if(pages[i].mapping&&drop_handle(fd,20,pages[i].mapping)){rc=1;goto disconnect;}
 if(pages[i].reservation&&drop_handle(fd,22,pages[i].reservation)){rc=1;goto disconnect;}
 if(pages[i].pmr&&drop_handle(fd,8,pages[i].pmr)){rc=1;goto disconnect;}
 }
 for(int i=4;i>=0;i--)if(arenas[i].handle&&drop_handle(fd,18,arenas[i].handle)){rc=1;goto disconnect;}
 if(co.server_memctx&&drop_handle(fd,16,co.server_memctx))rc=1;
disconnect:
 if(connected){struct pvr_srv_bridge_disconnect_ret d;if(pvr_srv_ddk19_bridge_call(fd,1,1,NULL,0,&d,sizeof(d))||d.error)rc=1;}
 close(fd);printf("COMPUTE_STORE_TRIAL_RC=%d pages=%u\n",rc,checked);return rc;
}
