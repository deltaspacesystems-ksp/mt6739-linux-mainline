from pathlib import Path
p=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h');s=p.read_text();Path('/mnt/d/y5_linux_work/out/pvr_srv_ddk19-stage3a.h').write_text(s)
helper='''/* Conservative subset: ordinary non-sparse 4K buffers. Never truncate flags. */
static bool mfw19_memory_flags(uint64_t flags, bool mapping, uint32_t *old)
{
 uint64_t mask=mapping?UINT64_C(0x70f):UINT64_C(0x80007fff);
 /* bit15/sparse/SVM/device/heap hints and other unverified modes rejected. */
 if(flags&~mask)return false;
 unsigned gpu=(flags>>8)&7,cpu=(flags>>11)&7;
 if(gpu!=1&&gpu!=3)return false;
 if(!mapping&&cpu!=0&&cpu!=3)return false;
 *old=(uint32_t)(flags & (mapping?0xf:UINT64_C(0x800040ff)));
 if(gpu==3)*old|=3u<<8; /* modern uncached1 -> old uncached0 */
 if(!mapping)*old|=(cpu==0?1u:3u)<<11; /* modern WC0 -> old WC1 */
 return true;
}
'''
s=s.replace('struct mfw19_connect_in',helper+'struct mfw19_connect_in',1)
anchor=' /* Never send an unchecked1.17 command/layout to a1.9 kernel. */'
code=''' if(bridge==PVR_SRV_BRIDGE_MM && function==10){
  struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd *in=input;
  struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret *out=output;
  struct {uint64_t size,chunk;uint32_t phys,virt;uint64_t table;uint32_t page,flags,annotation_size;uint64_t annotation;} PACKED old_in;
  struct {uint64_t pmr;uint32_t error;} PACKED old_out={.error=UINT32_MAX};
  uint32_t flags;
  _Static_assert(sizeof(old_in)==52&&sizeof(old_out)==12,"1.9 PMR allocation ABI");
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)||in->pid||in->pdump_flags||
     in->log2_page_size!=12||!in->size||(in->size&4095)||in->block_size!=in->size||
     in->phy_blocks!=1||in->virt_blocks!=1||!in->mapping_table||
     !in->annotation||!in->annotation_size||in->annotation_size>128||
     !mfw19_memory_flags(in->flags,false,&flags)){errno=EINVAL;return -1;}
  old_in.size=in->size;old_in.chunk=in->block_size;old_in.phys=1;old_in.virt=1;
  old_in.table=(uintptr_t)in->mapping_table;old_in.page=12;old_in.flags=flags;
  old_in.annotation_size=in->annotation_size;old_in.annotation=(uintptr_t)in->annotation;
  cmd.in_data_ptr=(uintptr_t)&old_in;cmd.in_data_size=sizeof(old_in);
  cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=sizeof(old_out);
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->pmr=(void*)(uintptr_t)old_out.pmr;out->error=mfw19_error(old_out.error);
   /* No kernel-returned flags in1.9. Unused by pinned Mesa; explicitly unavailable. */
   out->out_flags=0;}
  return rc;
 }
 if(bridge==PVR_SRV_BRIDGE_MM && function==19){
  struct pvr_srv_devmem_int_map_pmr_cmd *in=input;
  struct pvr_srv_devmem_int_map_pmr_ret *out=output;
  struct {uint64_t heap,reservation,pmr;uint32_t flags;} PACKED old_in;
  struct {uint64_t mapping;uint32_t error;} PACKED old_out={.error=UINT32_MAX};uint32_t flags;
  _Static_assert(sizeof(old_in)==28&&sizeof(old_out)==12,"1.9 PMR mapping ABI");
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)||!in->server_heap||!in->reservation||!in->pmr||
     !mfw19_memory_flags(in->flags,true,&flags)){errno=EINVAL;return -1;}
  old_in.heap=(uintptr_t)in->server_heap;old_in.reservation=(uintptr_t)in->reservation;old_in.pmr=(uintptr_t)in->pmr;old_in.flags=flags;
  cmd.in_data_ptr=(uintptr_t)&old_in;cmd.in_data_size=sizeof(old_in);
  cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=sizeof(old_out);
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->mapping=(void*)(uintptr_t)old_out.mapping;out->error=mfw19_error(old_out.error);}
  return rc;
 }
 if(bridge==PVR_SRV_BRIDGE_MM && (function==8||function==20)){
  uint64_t handle;uint32_t old_out=UINT32_MAX;
  if(insize!=8||outsize!=4){errno=EINVAL;return -1;}
  memcpy(&handle,input,8);if(!handle){errno=EINVAL;return -1;}
  cmd.in_data_ptr=(uintptr_t)&handle;cmd.in_data_size=8;
  cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=4;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){enum pvr_srv_error error=mfw19_error(old_out);memcpy(output,&error,4);}
  return rc;
 }
'''
assert anchor in s and '1.9 PMR allocation ABI' not in s;s=s.replace(anchor,code+'\n'+anchor);p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
# extend extracted structs in mocks and smoke
names=['pvr_srv_physmem_new_ram_backed_locked_pmr_cmd','pvr_srv_physmem_new_ram_backed_locked_pmr_ret','pvr_srv_devmem_int_map_pmr_cmd','pvr_srv_devmem_int_map_pmr_ret']
g=Path('/mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py');t=g.read_text();t=t.replace("'pvr_srv_devmem_int_reserve_range_ret']:","'pvr_srv_devmem_int_reserve_range_ret',"+','.join(repr(n) for n in names)+"]:")
t=t.replace('  else if(cmd->bridge_func_id==21)', '''  else if(cmd->bridge_func_id==10){assert(cmd->in_data_size==52&&cmd->out_data_size==12);uint8_t *i=(void*)(uintptr_t)cmd->in_data_ptr;uint64_t size,chunk,table,annotation;uint32_t phys,virt,page,flags,ann;memcpy(&size,i,8);memcpy(&chunk,i+8,8);memcpy(&phys,i+16,4);memcpy(&virt,i+20,4);memcpy(&table,i+24,8);memcpy(&page,i+32,4);memcpy(&flags,i+36,4);memcpy(&ann,i+40,4);memcpy(&annotation,i+44,8);assert(size==4096&&chunk==4096&&phys==1&&virt==1&&table&&page==12&&flags==0x80004b33&&ann==5&&annotation);uint64_t pmr=0x555;memcpy(b,&pmr,8);memcpy(b+8,&e,4);}
  else if(cmd->bridge_func_id==19){assert(cmd->in_data_size==28&&cmd->out_data_size==12);uint64_t input[3];uint32_t flags;memcpy(input,(void*)(uintptr_t)cmd->in_data_ptr,24);memcpy(&flags,(uint8_t*)(uintptr_t)cmd->in_data_ptr+24,4);assert(input[0]==0x333&&input[1]==0x444&&input[2]==0x555&&flags==0x303);uint64_t mapping=0x666;memcpy(b,&mapping,8);memcpy(b+8,&e,4);}
  else if(cmd->bridge_func_id==21)''')
t=t.replace('cmd->bridge_func_id==22||cmd->bridge_func_id==16','cmd->bridge_func_id==8||cmd->bridge_func_id==20||cmd->bridge_func_id==22||cmd->bridge_func_id==16')
anchor=' puts("PASS exact Mesa19 adapter:'
extra=''' uint32_t converted;
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
 previous=ioctls;ai.pid=1;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0&&ioctls==previous);ai.pid=0;
 ai.virt_blocks=2;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0&&ioctls==previous);ai.virt_blocks=1;
 ai.flags|=UINT64_C(1)<<60;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0&&ioctls==previous);ai.flags=0x80004333;
 struct pvr_srv_devmem_int_map_pmr_cmd mi={.server_heap=(void*)0x333,.pmr=(void*)0x555,.reservation=(void*)0x444,.flags=0x303};struct pvr_srv_devmem_int_map_pmr_ret mo;
 assert(!pvr_srv_ddk19_bridge_call(9,6,19,&mi,sizeof(mi),&mo,sizeof(mo))&&mo.error==0&&mo.mapping==(void*)0x666);
 previous=ioctls;mi.pmr=NULL;assert(pvr_srv_ddk19_bridge_call(9,6,19,&mi,sizeof(mi),&mo,sizeof(mo))<0&&ioctls==previous);mi.pmr=(void*)0x555;
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,6,19,&mi,sizeof(mi),&mo,sizeof(mo))&&mo.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))<0);mode=0;
 handle=0x666;assert(!pvr_srv_ddk19_bridge_call(9,6,20,&handle,8,&error,4)&&error==0);handle=0x555;
 mode=3;assert(!pvr_srv_ddk19_bridge_call(9,6,8,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);mode=0;
'''
assert anchor in t;t=t.replace(anchor,extra+anchor);g.write_text(t)
import re
q=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c');t=q.read_text();h=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h').read_text();structs='\n'.join(re.search(r'struct '+n+r' \{.*?\}\s*PACKED;',h,re.S).group() for n in names)
t=t.replace('#include "pvr_srv_ddk19.h"',structs+'\n#include "pvr_srv_ddk19.h"').replace('#include <fcntl.h>','#include <sys/mman.h>\n#include <fcntl.h>')
t=t.replace(' int connected=0,rc=1;', ''' uint32_t table=0;const char annotation[]="test";
 struct pvr_srv_physmem_new_ram_backed_locked_pmr_cmd ai={.block_size=4096,.size=4096,.mapping_table=&table,.annotation=annotation,.annotation_size=5,.log2_page_size=12,.phy_blocks=1,.virt_blocks=1,.flags=0x80004333};
 struct pvr_srv_physmem_new_ram_backed_locked_pmr_ret ao={0};
 struct pvr_srv_devmem_int_map_pmr_cmd mi={.flags=0x303};struct pvr_srv_devmem_int_map_pmr_ret mo={0};
 volatile uint32_t *memory=MAP_FAILED;
 int connected=0,rc=1;''')
t=t.replace(' printf("MESA19_NATIVE_RESERVATION_PASS', ''' if(pvr_srv_ddk19_bridge_call(fd,6,10,&ai,sizeof(ai),&ao,sizeof(ao))||ao.error||!ao.pmr)goto done;
 if((uintptr_t)ao.pmr>INT64_MAX/4096)goto done;
 memory=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)((uintptr_t)ao.pmr*4096));
 if(memory==MAP_FAILED)goto done;
 for(unsigned i=0;i<1024;i++)if(memory[i])goto done;
 for(unsigned i=0;i<1024;i++)memory[i]=0x4d465700u^(i*0x01010101u);
 __sync_synchronize();
 for(unsigned i=0;i<1024;i++)if(memory[i]!=(0x4d465700u^(i*0x01010101u)))goto done;
 mi.server_heap=heap_out.server_heap;mi.reservation=ro.reservation;mi.pmr=ao.pmr;
 if(pvr_srv_ddk19_bridge_call(fd,6,19,&mi,sizeof(mi),&mo,sizeof(mo))||mo.error||!mo.mapping)goto done;
 printf("MESA19_NATIVE_PMR_GPU_MAP_PASS''')
t=t.replace('done:\n','''done:
 if(memory!=MAP_FAILED&&munmap((void*)memory,4096))rc=1;
 if(mo.mapping&&drop_handle(fd,20,mo.mapping)){rc=1;goto disconnect;}
''')
t=t.replace(' if(heap_out.server_heap&&drop_handle', ' if(ao.pmr&&drop_handle(fd,8,ao.pmr)){rc=1;goto disconnect;}\n if(heap_out.server_heap&&drop_handle')
q.write_text(t)
print('Stage3b PMR/map adapter and tests prepared')
