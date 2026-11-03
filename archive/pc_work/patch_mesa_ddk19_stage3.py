from pathlib import Path
import re
root=Path('/home/y5/mesa-native');p=root/'src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h'
s=p.read_text();Path('/mnt/d/y5_linux_work/out/pvr_srv_ddk19-stage2.h').write_text(s)
anchor=' /* Never send an unchecked1.17 command/layout to a1.9 kernel. */'
assert anchor in s and '1.9 reservation ABI' not in s
code=''' if(bridge==PVR_SRV_BRIDGE_MM && function==21){
  struct pvr_srv_devmem_int_reserve_range_cmd *in=input;
  struct pvr_srv_devmem_int_reserve_range_ret *out=output;
  struct {uint64_t heap,address,length;} PACKED old_in;
  struct {uint64_t reservation;uint32_t error;} PACKED old_out={.error=UINT32_MAX};
  _Static_assert(sizeof(old_in)==24&&sizeof(old_out)==12,"1.9 reservation ABI");
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)||!in->server_heap||!in->size||
     (in->addr.addr&4095)||(in->size&4095)||in->addr.addr>UINT64_MAX-in->size){errno=EINVAL;return -1;}
  old_in.heap=(uintptr_t)in->server_heap;old_in.address=in->addr.addr;old_in.length=in->size;
  cmd.in_data_ptr=(uintptr_t)&old_in;cmd.in_data_size=sizeof(old_in);
  cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=sizeof(old_out);
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->reservation=(void*)(uintptr_t)old_out.reservation;out->error=mfw19_error(old_out.error);}
  return rc;
 }
 if(bridge==PVR_SRV_BRIDGE_MM && function==22){
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
s=s.replace(anchor,code+'\n'+anchor);p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
# Extend generated tests with modern Mesa structs, exact old-wire assertions and refusal cases.
g=Path('/mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py');t=g.read_text()
t=t.replace("'pvr_srv_devmem_int_heap_create_ret']:","'pvr_srv_devmem_int_heap_create_ret','pvr_srv_devmem_int_reserve_range_cmd','pvr_srv_devmem_int_reserve_range_ret']:")
t=t.replace('  else{assert((cmd->bridge_func_id==16', '''  else if(cmd->bridge_func_id==21){assert(cmd->in_data_size==24&&cmd->out_data_size==12);uint64_t input[3];memcpy(input,(void*)(uintptr_t)cmd->in_data_ptr,24);assert(input[0]==0x333&&input[1]==UINT64_C(0x65c0000000)&&input[2]==4096);uint64_t reservation=0x444;memcpy(b,&reservation,8);memcpy(b+8,&e,4);}
  else{assert((cmd->bridge_func_id==22||cmd->bridge_func_id==16''')
anchor=' puts("PASS exact Mesa19 adapter:'
extra=''' struct pvr_srv_devmem_int_reserve_range_cmd ri={.addr={.addr=UINT64_C(0x65c0000000)},.size=4096,.server_heap=(void*)0x333};
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
'''
assert anchor in t;t=t.replace(anchor,extra+anchor);g.write_text(t)
# Preserve the standalone phone wrapper, adding exact Mesa structures.
p=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c');s=p.read_text();h=(root/'src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h').read_text()
structs='\n'.join(re.search(r'struct '+n+r' \{.*?\}\s*PACKED;',h,re.S).group() for n in ['pvr_srv_devmem_int_reserve_range_cmd','pvr_srv_devmem_int_reserve_range_ret'])
s=s.replace('#include "pvr_srv_ddk19.h"',structs+'\n#include "pvr_srv_ddk19.h"')
s=s.replace(' int connected=0,rc=1;',' struct pvr_srv_devmem_int_reserve_range_cmd ri={.addr={.addr=UINT64_C(0x65c0000000)},.size=4096};\n struct pvr_srv_devmem_int_reserve_range_ret ro={0};\n int connected=0,rc=1;')
s=s.replace(' printf("MESA19_NATIVE_CONTEXT_PASS', ' ri.server_heap=heap_out.server_heap;\n if(pvr_srv_ddk19_bridge_call(fd,6,21,&ri,sizeof(ri),&ro,sizeof(ro))||ro.error||!ro.reservation)goto done;\n printf("MESA19_NATIVE_RESERVATION_PASS bytes=4096; context_PASS')
s=s.replace('done:\n','done:\n if(ro.reservation&&drop_handle(fd,22,ro.reservation)){rc=1;goto disconnect;}\n')
p.write_text(s)
print('Stage3 reservation adapter/tests/smoke prepared')
