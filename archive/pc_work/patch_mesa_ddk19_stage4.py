from pathlib import Path
import re
p=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h');s=p.read_text();Path('/mnt/d/y5_linux_work/out/pvr_srv_ddk19-stage3b.h').write_text(s)
code=''' if(bridge==2 && function==0){
  struct pvr_srv_bridge_alloc_sync_primitive_block_ret *out=output;
  uint32_t placeholder=0;
  struct {uint64_t handle;uint32_t address,size;uint64_t export_handle;uint32_t error;} PACKED old={.error=UINT32_MAX};
  _Static_assert(sizeof(old)==28,"1.9 sync allocation ABI");
  if(insize!=0||outsize!=sizeof(*out)){errno=EINVAL;return -1;}
  cmd.in_data_ptr=(uintptr_t)&placeholder;cmd.in_data_size=4;
  cmd.out_data_ptr=(uintptr_t)&old;cmd.out_data_size=28;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->handle=(void*)(uintptr_t)old.handle;out->pmr=(void*)(uintptr_t)old.export_handle;
   out->addr=old.address;out->size=old.size;out->error=mfw19_error(old.error);}
  return rc;
 }
 if(bridge==2 && (function==1||function==2)){
  uint32_t old_error=UINT32_MAX;
  if(outsize!=4||insize!=(function==1?8u:16u)){errno=EINVAL;return -1;}
  uint64_t handle;memcpy(&handle,input,8);if(!handle){errno=EINVAL;return -1;}
  /* SET input layout is identical: handle64/index32/value32. Kernel checks bounds. */
  cmd.in_data_ptr=(uintptr_t)input;cmd.in_data_size=insize;
  cmd.out_data_ptr=(uintptr_t)&old_error;cmd.out_data_size=4;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){enum pvr_srv_error error=mfw19_error(old_error);memcpy(output,&error,4);}
  return rc;
 }
'''
a=' /* Never send an unchecked1.17 command/layout to a1.9 kernel. */';assert a in s and '1.9 sync allocation ABI' not in s;s=s.replace(a,code+'\n'+a);p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
names=['pvr_srv_bridge_alloc_sync_primitive_block_ret','pvr_srv_bridge_sync_prim_set_cmd']
g=Path('/mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py');t=g.read_text();t=t.replace("'pvr_srv_devmem_int_map_pmr_ret']:","'pvr_srv_devmem_int_map_pmr_ret',"+','.join(repr(n) for n in names)+"]:")
a=' if(cmd->bridge_id==6){';extra=''' if(cmd->bridge_id==2){
  uint8_t *out=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t error=mode==2?5:mode==3?25:0;
  if(cmd->bridge_func_id==0){assert(cmd->in_data_size==4&&cmd->out_data_size==28&&*(uint32_t*)(uintptr_t)cmd->in_data_ptr==0);uint64_t handle=0x777,export_handle=0x888;uint32_t address=0xa000,size=4096;memcpy(out,&handle,8);memcpy(out+8,&address,4);memcpy(out+12,&size,4);memcpy(out+16,&export_handle,8);memcpy(out+24,&error,4);}
  else{assert(cmd->out_data_size==4);uint64_t handle;memcpy(&handle,(void*)(uintptr_t)cmd->in_data_ptr,8);assert(handle==0x777);if(cmd->bridge_func_id==2){assert(cmd->in_data_size==16);uint32_t fields[2];memcpy(fields,(uint8_t*)(uintptr_t)cmd->in_data_ptr+8,8);assert(fields[0]==0&&fields[1]==0x12345678);}else assert(cmd->bridge_func_id==1&&cmd->in_data_size==8);memcpy(out,&error,4);}
  return 0;
 }
''';assert a in t;t=t.replace(a,extra+a)
a=' puts("PASS exact Mesa19 adapter:';extra=''' struct pvr_srv_bridge_alloc_sync_primitive_block_ret sync;
 assert(!pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync))&&sync.error==0&&sync.handle==(void*)0x777&&sync.pmr==(void*)0x888&&sync.addr==0xa000&&sync.size==4096);
 previous=ioctls;assert(pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync)-1)<0&&ioctls==previous);
 struct pvr_srv_bridge_sync_prim_set_cmd si={.handle=(void*)0x777,.index=0,.value=0x12345678};
 assert(!pvr_srv_ddk19_bridge_call(9,2,2,&si,sizeof(si),&error,4)&&error==0);
 previous=ioctls;si.handle=NULL;assert(pvr_srv_ddk19_bridge_call(9,2,2,&si,sizeof(si),&error,4)<0&&ioctls==previous);si.handle=(void*)0x777;
 handle=0x777;assert(!pvr_srv_ddk19_bridge_call(9,2,1,&handle,8,&error,4)&&error==0);
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync))&&sync.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 mode=3;assert(!pvr_srv_ddk19_bridge_call(9,2,1,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);
 mode=1;assert(pvr_srv_ddk19_bridge_call(9,2,0,NULL,0,&sync,sizeof(sync))<0);mode=0;
''';t=t.replace(a,extra+a);g.write_text(t)
p=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c');t=p.read_text();h=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h').read_text();structs='\n'.join(re.search(r'struct '+n+r' \{.*?\}\s*PACKED;',h,re.S).group() for n in names)
t=t.replace('#include "pvr_srv_ddk19.h"',structs+'\n#include "pvr_srv_ddk19.h"')
t=t.replace(' int connected=0,rc=1;',' struct pvr_srv_bridge_alloc_sync_primitive_block_ret sync={0};\n struct pvr_srv_bridge_sync_prim_set_cmd si={.index=0,.value=0x12345678};enum pvr_srv_error sync_error;\n int connected=0,rc=1;')
a=' printf("MESA19_NATIVE_PMR_GPU_MAP_PASS';extra=''' if(pvr_srv_ddk19_bridge_call(fd,2,0,NULL,0,&sync,sizeof(sync))||sync.error||!sync.handle||!sync.pmr||sync.size<4)goto done;
 si.handle=sync.handle;
 if(pvr_srv_ddk19_bridge_call(fd,2,2,&si,sizeof(si),&sync_error,4)||sync_error)goto done;
 si.value=0;
 if(pvr_srv_ddk19_bridge_call(fd,2,2,&si,sizeof(si),&sync_error,4)||sync_error)goto done;
 printf("MESA19_SYNC_ALLOC_SET_PASS size=%u addr=%x; no GPU completion tested\\n",sync.size,sync.addr);
''';assert a in t;t=t.replace(a,extra+a)
t=t.replace('done:\n','''done:
 if(sync.handle){enum pvr_srv_error e=PVR_SRV_ERROR_BRIDGE_CALL_FAILED;
  if(pvr_srv_ddk19_bridge_call(fd,2,1,&sync.handle,8,&e,4)||e){rc=1;goto disconnect;}}
''');p.write_text(t)
print('sync adapter and exact wire tests prepared')
