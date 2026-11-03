from pathlib import Path
import re
r=Path('/home/y5/mesa-native');h=(r/'src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h').read_text()
source='''#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <xf86drm.h>
#define PACKED __attribute__((packed))
#define PVR_MFW_DDK19 1
typedef struct {uint64_t addr;} pvr_dev_addr_t;
'''
for name in ['PVR_SRV_BRIDGE_SRVCORE','PVR_SRV_BRIDGE_SRVCORE_CONNECT','PVR_SRV_BRIDGE_SRVCORE_DISCONNECT','PVR_SRV_BRIDGE_MM','PVR_SRV_BRIDGE_MM_DEVMEMINTCTXCREATE','PVR_SRV_BRIDGE_MM_DEVMEMINTCTXDESTROY','PVR_SRV_BRIDGE_MM_HEAPCFGHEAPCOUNT','PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPCREATE','PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPDESTROY']:
 source+=re.search(r'^#define '+name+r'\s+[^\n]+',h,re.M).group()+'\n'
source+=re.search(r'enum pvr_srv_error \{.*?\};',h,re.S).group()+'\n'
for name in ['drm_srvkm_cmd','pvr_srv_bridge_connect_cmd','pvr_srv_bridge_connect_ret','pvr_srv_bridge_disconnect_ret','pvr_srv_devmem_int_ctx_create_cmd','pvr_srv_devmem_int_ctx_create_ret','pvr_srv_heap_count_cmd','pvr_srv_heap_count_ret','pvr_srv_devmem_int_heap_create_cmd','pvr_srv_devmem_int_heap_create_ret','pvr_srv_devmem_int_reserve_range_cmd','pvr_srv_devmem_int_reserve_range_ret','pvr_srv_physmem_new_ram_backed_locked_pmr_cmd','pvr_srv_physmem_new_ram_backed_locked_pmr_ret','pvr_srv_devmem_int_map_pmr_cmd','pvr_srv_devmem_int_map_pmr_ret','pvr_srv_bridge_alloc_sync_primitive_block_ret','pvr_srv_bridge_sync_prim_set_cmd','pvr_srv_rgx_create_transfer_context_cmd','pvr_srv_rgx_create_transfer_context_ret']:
 source+=re.search(r'struct '+name+r' \{.*?\}\s*(?:PACKED)?;',h,re.S).group()+'\n'
source+='''
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
 if(cmd->bridge_id==128){
  uint8_t *out=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t error=mode==2?5:mode==3?25:0;
  if(cmd->bridge_func_id==0){assert(cmd->in_data_size==24&&cmd->out_data_size==12);uint8_t*i=(void*)(uintptr_t)cmd->in_data_ptr;uint32_t priority,size;uint64_t framework,priv;memcpy(&priority,i,4);memcpy(&size,i+4,4);memcpy(&framework,i+8,8);memcpy(&priv,i+16,8);assert(priority==0&&size==16&&framework&&priv==0x222);uint64_t context=0x999;memcpy(out,&context,8);memcpy(out+8,&error,4);}
  else{assert(cmd->bridge_func_id==1&&cmd->in_data_size==8&&cmd->out_data_size==4);memcpy(out,&error,4);}return 0;
 }
 if(cmd->bridge_id==2){
  uint8_t *out=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t error=mode==2?5:mode==3?25:0;
  if(cmd->bridge_func_id==0){assert(cmd->in_data_size==4&&cmd->out_data_size==28&&*(uint32_t*)(uintptr_t)cmd->in_data_ptr==0);uint64_t handle=0x777,export_handle=0x888;uint32_t address=0xa000,size=4096;memcpy(out,&handle,8);memcpy(out+8,&address,4);memcpy(out+12,&size,4);memcpy(out+16,&export_handle,8);memcpy(out+24,&error,4);}
  else{assert(cmd->out_data_size==4);uint64_t handle;memcpy(&handle,(void*)(uintptr_t)cmd->in_data_ptr,8);assert(handle==0x777);if(cmd->bridge_func_id==2){assert(cmd->in_data_size==16);uint32_t fields[2];memcpy(fields,(uint8_t*)(uintptr_t)cmd->in_data_ptr+8,8);assert(fields[0]==0&&fields[1]==0x12345678);}else assert(cmd->bridge_func_id==1&&cmd->in_data_size==8);memcpy(out,&error,4);}
  return 0;
 }
 if(cmd->bridge_id==6){
  uint8_t *b=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t e=mode==2?5:mode==3?25:0;
  memset(b,0,cmd->out_data_size);
  if(cmd->bridge_func_id==15){assert(cmd->in_data_size==4&&cmd->out_data_size==24);uint64_t handles[2]={0x111,0x222};uint32_t line=64;memcpy(b,handles,16);memcpy(b+16,&line,4);memcpy(b+20,&e,4);}
  else if(cmd->bridge_func_id==28){assert(cmd->in_data_size==4&&cmd->out_data_size==8);uint32_t count=17;memcpy(b,&count,4);memcpy(b+4,&e,4);}
  else if(cmd->bridge_func_id==17){assert(cmd->in_data_size==28&&cmd->out_data_size==12);uint8_t *i=(void*)(uintptr_t)cmd->in_data_ptr;uint64_t context,base,size;uint32_t page;memcpy(&context,i,8);memcpy(&base,i+8,8);memcpy(&size,i+16,8);memcpy(&page,i+24,4);assert(context==0x111&&base==UINT64_C(0x65c0000000)&&size==0x80000000&&page==12);uint64_t heap=0x333;memcpy(b,&heap,8);memcpy(b+8,&e,4);}
  else if(cmd->bridge_func_id==10){assert(cmd->in_data_size==52&&cmd->out_data_size==12);uint8_t *i=(void*)(uintptr_t)cmd->in_data_ptr;uint64_t size,chunk,table,annotation;uint32_t phys,virt,page,flags,ann;memcpy(&size,i,8);memcpy(&chunk,i+8,8);memcpy(&phys,i+16,4);memcpy(&virt,i+20,4);memcpy(&table,i+24,8);memcpy(&page,i+32,4);memcpy(&flags,i+36,4);memcpy(&ann,i+40,4);memcpy(&annotation,i+44,8);assert(size==4096&&chunk==4096&&phys==1&&virt==1&&table&&page==12&&flags==0x80004b33&&ann==5&&annotation);uint64_t pmr=0x555;memcpy(b,&pmr,8);memcpy(b+8,&e,4);}
  else if(cmd->bridge_func_id==19){assert(cmd->in_data_size==28&&cmd->out_data_size==12);uint64_t input[3];uint32_t flags;memcpy(input,(void*)(uintptr_t)cmd->in_data_ptr,24);memcpy(&flags,(uint8_t*)(uintptr_t)cmd->in_data_ptr+24,4);assert(input[0]==0x333&&input[1]==0x444&&input[2]==0x555&&flags==0x303);uint64_t mapping=0x666;memcpy(b,&mapping,8);memcpy(b+8,&e,4);}
  else if(cmd->bridge_func_id==21){assert(cmd->in_data_size==24&&cmd->out_data_size==12);uint64_t input[3];memcpy(input,(void*)(uintptr_t)cmd->in_data_ptr,24);assert(input[0]==0x333&&input[1]==UINT64_C(0x65c0000000)&&input[2]==4096);uint64_t reservation=0x444;memcpy(b,&reservation,8);memcpy(b+8,&e,4);}
  else{assert((cmd->bridge_func_id==8||cmd->bridge_func_id==20||cmd->bridge_func_id==22||cmd->bridge_func_id==16||cmd->bridge_func_id==18)&&cmd->in_data_size==8&&cmd->out_data_size==4);memcpy(b,&e,4);}
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
 struct pvr_srv_devmem_int_reserve_range_cmd ri={.addr={.addr=UINT64_C(0x65c0000000)},.size=4096,.server_heap=(void*)0x333};
 struct pvr_srv_devmem_int_reserve_range_ret ro;
 assert(!pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))&&ro.error==0&&ro.reservation==(void*)0x444);
 previous=ioctls;
 ri.size=0;assert(pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))<0&&ioctls==previous);ri.size=4096;
 ri.addr.addr++;assert(pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))<0&&ioctls==previous);ri.addr.addr--;
 ri.addr.addr=UINT64_MAX-4095;assert(pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))<0&&ioctls==previous);ri.addr.addr=UINT64_C(0x65c0000000);
 ri.server_heap=NULL;assert(pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))<0&&ioctls==previous);ri.server_heap=(void*)0x333;
 assert(pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri)-1,&ro,sizeof(ro))<0&&ioctls==previous);
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))&&ro.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,6,21,&ri,sizeof(ri),&ro,sizeof(ro))<0);mode=0;
 handle=0x444;assert(!pvr_srv_ddk19_bridge_call(9,6,22,&handle,8,&error,4)&&error==0);
 mode=3;assert(!pvr_srv_ddk19_bridge_call(9,6,22,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);mode=0;
 handle=0;previous=ioctls;assert(pvr_srv_ddk19_bridge_call(9,6,22,&handle,8,&error,4)<0&&ioctls==previous);
 uint32_t converted;
 for(unsigned gpu=0;gpu<8;gpu++)for(unsigned cpu=0;cpu<8;cpu++){
  bool ok=mfw19_memory_flags(0x80004033u|((uint64_t)gpu<<8)|((uint64_t)cpu<<11),false,&converted);
  assert(ok==((gpu==1||gpu==3)&&(cpu==0||cpu==3)));
  if(ok)assert(converted==(0x80004033u|(gpu==3?0x300:0)|(cpu==0?0x800:0x1800)));
 }
 for(unsigned bit=0;bit<64;bit++)if(!(UINT64_C(0x80007fff)&(UINT64_C(1)<<bit)))assert(!mfw19_memory_flags(UINT64_C(0x80004333)|(UINT64_C(1)<<bit),false,&converted));
 assert(!mfw19_memory_flags(UINT64_C(0x4333),true,&converted));
 uint32_t table=0;char annotation[]="test";
 struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd ai={.block_size=4096,.size=4096,.mapping_table=&table,.annotation=annotation,.annotation_size=5,.log2_page_size=12,.phy_blocks=1,.virt_blocks=1,.flags=0x80004333};
 struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret ao;
 assert(!pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))&&ao.error==0&&ao.pmr==(void*)0x555&&ao.out_flags==0);
 ai.pid=(uint32_t)getpid();assert(!pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))&&ao.error==0);
 previous=ioctls;ai.pid=(uint32_t)getpid()+1;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0&&ioctls==previous);ai.pid=0;
 ai.virt_blocks=2;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0&&ioctls==previous);ai.virt_blocks=1;
 ai.flags|=UINT64_C(1)<<60;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0&&ioctls==previous);ai.flags=0x80004333;
 struct pvr_srv_devmem_int_map_pmr_cmd mi={.server_heap=(void*)0x333,.pmr=(void*)0x555,.reservation=(void*)0x444,.flags=0x303};struct pvr_srv_devmem_int_map_pmr_ret mo;
 assert(!pvr_srv_ddk19_bridge_call(9,6,19,&mi,sizeof(mi),&mo,sizeof(mo))&&mo.error==0&&mo.mapping==(void*)0x666);
 previous=ioctls;mi.pmr=NULL;assert(pvr_srv_ddk19_bridge_call(9,6,19,&mi,sizeof(mi),&mo,sizeof(mo))<0&&ioctls==previous);mi.pmr=(void*)0x555;
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,6,19,&mi,sizeof(mi),&mo,sizeof(mo))&&mo.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0);mode=0;
 handle=0x666;assert(!pvr_srv_ddk19_bridge_call(9,6,20,&handle,8,&error,4)&&error==0);handle=0x555;
 mode=3;assert(!pvr_srv_ddk19_bridge_call(9,6,8,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);mode=0;
 struct pvr_srv_bridge_alloc_sync_primitive_block_ret sync;
 assert(!pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync))&&sync.error==0&&sync.handle==(void*)0x777&&sync.pmr==(void*)0x888&&sync.addr==0xa000&&sync.size==4096);
 previous=ioctls;assert(pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync)-1)<0&&ioctls==previous);
 struct pvr_srv_bridge_sync_prim_set_cmd si={.handle=(void*)0x777,.index=0,.value=0x12345678};
 assert(!pvr_srv_ddk19_bridge_call(9,2,2,&si,sizeof(si),&error,4)&&error==0);
 previous=ioctls;si.handle=NULL;assert(pvr_srv_ddk19_bridge_call(9,2,2,&si,sizeof(si),&error,4)<0&&ioctls==previous);si.handle=(void*)0x777;
 handle=0x777;assert(!pvr_srv_ddk19_bridge_call(9,2,1,&handle,8,&error,4)&&error==0);
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync))&&sync.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 mode=3;assert(!pvr_srv_ddk19_bridge_call(9,2,1,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync))<0);mode=0;
 uint8_t reset[16]={0};struct pvr_srv_rgx_create_transfer_context_cmd ti={.priv_data=(void*)0x222,.reset_framework_cmd=reset,.reset_framework_cmd_size=16};struct pvr_srv_rgx_create_transfer_context_ret to;
 assert(!pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))&&to.error==0&&to.transfer_context==(void*)0x999&&!to.cli_pmr_mem&&!to.usc_pmr_mem);
 previous=ioctls;reset[0]=1;assert(pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))<0&&ioctls==previous);reset[0]=0;
 ti.robustness_address=1;assert(pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))<0&&ioctls==previous);ti.robustness_address=0;
 ti.packed_ccb_size_u8888=1;assert(pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))<0&&ioctls==previous);ti.packed_ccb_size_u8888=0;
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))&&to.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 handle=0x999;mode=3;assert(!pvr_srv_ddk19_bridge_call(9,128,1,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);mode=0;
 puts("PASS exact Mesa19 adapter: typed CONNECT/DISCONNECT, driver guards, error translation and fail-closed unsupported bridge");
}
'''
Path('/mnt/d/y5_linux_work/test_mesa_ddk19_adapter.c').write_text(source)
