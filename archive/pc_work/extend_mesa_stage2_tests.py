from pathlib import Path
p=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h');s=p.read_text();s=s.replace('#ifdef PVR_MFW_DDK19\n','#ifdef PVR_MFW_DDK19\nstatic enum pvr_srv_error mfw19_error(uint32_t error)\n{\n /*0/25 confirmed with the generated1.9 enum; other codes remain opaque. */\n return error==0?PVR_SRV_OK:error==25?PVR_SRV_ERROR_RETRY:PVR_SRV_ERROR_BRIDGE_CALL_FAILED;\n}\n',1);s=s.replace(' struct drm_srvkm_cmd cmd={.bridge_id=bridge,.bridge_func_id=function};',' struct drm_srvkm_cmd cmd={.bridge_id=bridge,.bridge_func_id=function};\n if((insize&&!input)||(outsize&&!output)){errno=EINVAL;return -1;}')
for old,new in [('result.error==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED','mfw19_error(result.error)'),('legacy_out==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED','mfw19_error(legacy_out)'),('old.error==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED','mfw19_error(old.error)'),('old_out.error==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED','mfw19_error(old_out.error)'),('old_out==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED','mfw19_error(old_out)')]:s=s.replace(old,new)
p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
p=Path('/mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py');s=p.read_text().replace("#define PVR_MFW_DDK19 1", "#define PVR_MFW_DDK19 1\ntypedef struct {uint64_t addr;} pvr_dev_addr_t;")
s=s.replace("'PVR_SRV_BRIDGE_SRVCORE_DISCONNECT']", "'PVR_SRV_BRIDGE_SRVCORE_DISCONNECT','PVR_SRV_BRIDGE_MM','PVR_SRV_BRIDGE_MM_DEVMEMINTCTXCREATE','PVR_SRV_BRIDGE_MM_DEVMEMINTCTXDESTROY','PVR_SRV_BRIDGE_MM_HEAPCFGHEAPCOUNT','PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPCREATE','PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPDESTROY']")
s=s.replace("'pvr_srv_bridge_disconnect_ret']", "'pvr_srv_bridge_disconnect_ret','pvr_srv_devmem_int_ctx_create_cmd','pvr_srv_devmem_int_ctx_create_ret','pvr_srv_heap_count_cmd','pvr_srv_heap_count_ret','pvr_srv_devmem_int_heap_create_cmd','pvr_srv_devmem_int_heap_create_ret']")
s=s.replace(' if(cmd->bridge_func_id==PVR_SRV_BRIDGE_SRVCORE_CONNECT){',''' if(cmd->bridge_id==6){
  uint8_t *b=(void*)(uintptr_t)cmd->out_data_ptr;uint32_t e=mode==2?5:mode==3?25:0;
  memset(b,0,cmd->out_data_size);
  if(cmd->bridge_func_id==15){assert(cmd->in_data_size==4&&cmd->out_data_size==24);uint64_t handles[2]={0x111,0x222};uint32_t line=64;memcpy(b,handles,16);memcpy(b+16,&line,4);memcpy(b+20,&e,4);}
  else if(cmd->bridge_func_id==28){assert(cmd->in_data_size==4&&cmd->out_data_size==8);uint32_t count=17;memcpy(b,&count,4);memcpy(b+4,&e,4);}
  else if(cmd->bridge_func_id==17){assert(cmd->in_data_size==28&&cmd->out_data_size==12);uint8_t *i=(void*)(uintptr_t)cmd->in_data_ptr;uint64_t context,base,size;uint32_t page;memcpy(&context,i,8);memcpy(&base,i+8,8);memcpy(&size,i+16,8);memcpy(&page,i+24,4);assert(context==0x111&&base==UINT64_C(0x65c0000000)&&size==0x80000000&&page==12);uint64_t heap=0x333;memcpy(b,&heap,8);memcpy(b+8,&e,4);}
  else{assert((cmd->bridge_func_id==16||cmd->bridge_func_id==18)&&cmd->in_data_size==8&&cmd->out_data_size==4);memcpy(b,&e,4);}
  return 0;
 }
 if(cmd->bridge_func_id==PVR_SRV_BRIDGE_SRVCORE_CONNECT){''')
s=s.replace('pvr_srv_ddk19_bridge_call(9,6,30,','pvr_srv_ddk19_bridge_call(9,6,32,')
needle=' puts("PASS exact Mesa19 adapter:'
pos=s.index(needle)
s=s[:pos]+''' mode=0;
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
'''+s[pos:]
p.write_text(s)
