from pathlib import Path
import re
p=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h');s=p.read_text();Path('/mnt/d/y5_linux_work/out/pvr_srv_ddk19-pre-transfer.h').write_text(s)
code=''' if(bridge==128 && function==0){
  struct pvr_srv_rgx_create_transfer_context_cmd *in=input;
  struct pvr_srv_rgx_create_transfer_context_ret *out=output;
  struct {uint32_t priority,size;uint64_t framework,private_data;} PACKED old_in;
  struct {uint64_t context;uint32_t error;} PACKED old_out={.error=UINT32_MAX};
  _Static_assert(sizeof(old_in)==24&&sizeof(old_out)==12,"1.9 transfer context ABI");
  /* Only the disabled16-byte reset-framework diagnostic is currently supported. */
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)||!in->priv_data||in->robustness_address||in->context_flags||in->packed_ccb_size_u8888||in->priority||in->reset_framework_cmd_size!=16||!in->reset_framework_cmd){errno=EINVAL;return -1;}
  for(unsigned i=0;i<16;i++)if(in->reset_framework_cmd[i]){errno=EINVAL;return -1;}
  old_in.priority=0;old_in.size=16;old_in.framework=(uintptr_t)in->reset_framework_cmd;old_in.private_data=(uintptr_t)in->priv_data;
  cmd.in_data_ptr=(uintptr_t)&old_in;cmd.in_data_size=24;cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=12;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->transfer_context=(void*)(uintptr_t)old_out.context;out->cli_pmr_mem=NULL;out->usc_pmr_mem=NULL;out->error=mfw19_error(old_out.error);}
  return rc;
 }
 if(bridge==128 && function==1){
  uint64_t handle;uint32_t old_error=UINT32_MAX;
  if(insize!=8||outsize!=4){errno=EINVAL;return -1;}
  memcpy(&handle,input,8);if(!handle){errno=EINVAL;return -1;}
  cmd.in_data_ptr=(uintptr_t)&handle;cmd.in_data_size=8;cmd.out_data_ptr=(uintptr_t)&old_error;cmd.out_data_size=4;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){enum pvr_srv_error error=mfw19_error(old_error);memcpy(output,&error,4);}
  return rc;
 }
'''
a=' /* Never send an unchecked1.17 command/layout to a1.9 kernel. */';assert a in s and '1.9 transfer context ABI' not in s;s=s.replace(a,code+'\n'+a);p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
names=['pvr_srv_rgx_create_transfer_context_cmd','pvr_srv_rgx_create_transfer_context_ret']
g=Path('/mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py');t=g.read_text();t=t.replace("'pvr_srv_bridge_sync_prim_set_cmd']:","'pvr_srv_bridge_sync_prim_set_cmd',"+','.join(repr(n) for n in names)+"]:")
a=' if(cmd->bridge_id==2){';extra=''' if(cmd->bridge_id==128){
  uint8_t *out=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t error=mode==2?5:mode==3?25:0;
  if(cmd->bridge_func_id==0){assert(cmd->in_data_size==24&&cmd->out_data_size==12);uint8_t*i=(void*)(uintptr_t)cmd->in_data_ptr;uint32_t priority,size;uint64_t framework,priv;memcpy(&priority,i,4);memcpy(&size,i+4,4);memcpy(&framework,i+8,8);memcpy(&priv,i+16,8);assert(priority==0&&size==16&&framework&&priv==0x222);uint64_t context=0x999;memcpy(out,&context,8);memcpy(out+8,&error,4);}
  else{assert(cmd->bridge_func_id==1&&cmd->in_data_size==8&&cmd->out_data_size==4);memcpy(out,&error,4);}return 0;
 }
''';assert a in t;t=t.replace(a,extra+a)
a=' puts("PASS exact Mesa19 adapter:';extra=''' uint8_t reset[16]={0};struct pvr_srv_rgx_create_transfer_context_cmd ti={.priv_data=(void*)0x222,.reset_framework_cmd=reset,.reset_framework_cmd_size=16};struct pvr_srv_rgx_create_transfer_context_ret to;
 assert(!pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))&&to.error==0&&to.transfer_context==(void*)0x999&&!to.cli_pmr_mem&&!to.usc_pmr_mem);
 previous=ioctls;reset[0]=1;assert(pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))<0&&ioctls==previous);reset[0]=0;
 ti.robustness_address=1;assert(pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))<0&&ioctls==previous);ti.robustness_address=0;
 ti.packed_ccb_size_u8888=1;assert(pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))<0&&ioctls==previous);ti.packed_ccb_size_u8888=0;
 mode=2;assert(!pvr_srv_ddk19_bridge_call(9,128,0,&ti,sizeof(ti),&to,sizeof(to))&&to.error==PVR_SRV_ERROR_BRIDGE_CALL_FAILED);
 handle=0x999;mode=3;assert(!pvr_srv_ddk19_bridge_call(9,128,1,&handle,8,&error,4)&&error==PVR_SRV_ERROR_RETRY);mode=0;
''';t=t.replace(a,extra+a);g.write_text(t)
p=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c');t=p.read_text();h=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h').read_text();structs='\n'.join(re.search(r'struct '+n+r' \{.*?\}\s*PACKED;',h,re.S).group() for n in names)
t=t.replace('#include "pvr_srv_ddk19.h"',structs+'\n#include "pvr_srv_ddk19.h"').replace(' int connected=0,rc=1;', ' uint8_t reset[16]={0};struct pvr_srv_rgx_create_transfer_context_cmd ti={.reset_framework_cmd=reset,.reset_framework_cmd_size=16};struct pvr_srv_rgx_create_transfer_context_ret to={0};\n int connected=0,rc=1;')
a=' printf("MESA19_NATIVE_PMR_GPU_MAP_PASS';extra=''' ti.priv_data=co.server_memctx_data;
 if(pvr_srv_ddk19_bridge_call(fd,128,0,&ti,sizeof(ti),&to,sizeof(to))||to.error||!to.transfer_context){fprintf(stderr,"transfer create error=%u\\n",to.error);goto done;}
 printf("MESA19_TRANSFER_CONTEXT_CREATE_PASS reset-framework disabled; no submit\\n");
''';assert a in t;t=t.replace(a,extra+a)
t=t.replace('done:\n if(sync.handle)', '''done:
 if(to.transfer_context){int freed=0;for(unsigned i=0;i<50;i++){enum pvr_srv_error e=PVR_SRV_ERROR_BRIDGE_CALL_FAILED;
  if(pvr_srv_ddk19_bridge_call(fd,128,1,&to.transfer_context,8,&e,4))break;
  if(!e){freed=1;break;}if(e!=PVR_SRV_ERROR_RETRY)break;
  struct timespec pause={.tv_nsec=10000000};nanosleep(&pause,NULL);}
  if(!freed){rc=1;goto disconnect;}puts("MESA19_TRANSFER_CONTEXT_FREE_PASS");}
 if(sync.handle)''');p.write_text(t)
