 if(bridge==PVR_SRV_BRIDGE_MM && function==PVR_SRV_BRIDGE_MM_DEVMEMINTCTXCREATE){
  struct pvr_srv_devmem_int_ctx_create_cmd *in=input;
  struct pvr_srv_devmem_int_ctx_create_ret *out=output;
  struct {uint64_t context,private_data;uint32_t cache_line,error;} PACKED old={.error=UINT32_MAX};
  _Static_assert(sizeof(old)==24,"1.9 context output");
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)||in->kernel_memory_ctx){errno=EINVAL;return -1;}
  cmd.in_data_ptr=(uintptr_t)in;cmd.in_data_size=4;
  cmd.out_data_ptr=(uintptr_t)&old;cmd.out_data_size=24;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->server_memctx=(void*)(uintptr_t)old.context;out->server_memctx_data=(void*)(uintptr_t)old.private_data;out->cpu_cache_line_size=old.cache_line;out->error=old.error==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED;}
  return rc;
 }
 if(bridge==PVR_SRV_BRIDGE_MM && function==PVR_SRV_BRIDGE_MM_HEAPCFGHEAPCOUNT){
  struct pvr_srv_heap_count_cmd *in=input;
  struct pvr_srv_heap_count_ret *out=output;
  struct {uint32_t count,error;} PACKED old={.error=UINT32_MAX};
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)){errno=EINVAL;return -1;}
  cmd.bridge_func_id=28; /*1.9 HEAPCOUNT, not1.17's30*/
  cmd.in_data_ptr=(uintptr_t)in;cmd.in_data_size=4;
  cmd.out_data_ptr=(uintptr_t)&old;cmd.out_data_size=8;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->heap_count=old.count;out->error=old.error==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED;}
  return rc;
 }
 if(bridge==PVR_SRV_BRIDGE_MM && function==PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPCREATE){
  struct pvr_srv_devmem_int_heap_create_cmd *in=input;
  struct pvr_srv_devmem_int_heap_create_ret *out=output;
  struct {uint64_t context,base,size;uint32_t page;} PACKED old_in;
  struct {uint64_t heap;uint32_t error;} PACKED old_out={.error=UINT32_MAX};
  _Static_assert(sizeof(old_in)==28&&sizeof(old_out)==12,"1.9 heap ABI");
  if(insize!=sizeof(*in)||outsize!=sizeof(*out)||in->log2_page_size!=12||!in->size||(in->base_addr.addr&4095)||(in->size&4095)){errno=EINVAL;return -1;}
  old_in.context=(uintptr_t)in->server_memctx;old_in.base=in->base_addr.addr;old_in.size=in->size;old_in.page=in->log2_page_size;
  cmd.in_data_ptr=(uintptr_t)&old_in;cmd.in_data_size=28;
  cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=12;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){out->server_heap=(void*)(uintptr_t)old_out.heap;out->error=old_out.error==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED;}
  return rc;
 }
 if(bridge==PVR_SRV_BRIDGE_MM &&
    (function==PVR_SRV_BRIDGE_MM_DEVMEMINTCTXDESTROY||function==PVR_SRV_BRIDGE_MM_DEVMEMINTHEAPDESTROY)){
  uint64_t handle;uint32_t old_out=UINT32_MAX;
  if(insize!=8||outsize!=4){errno=EINVAL;return -1;}
  memcpy(&handle,input,8);
  cmd.in_data_ptr=(uintptr_t)&handle;cmd.in_data_size=8;
  cmd.out_data_ptr=(uintptr_t)&old_out;cmd.out_data_size=4;
  int rc=drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
  if(!rc){enum pvr_srv_error error=old_out==0?PVR_SRV_OK:PVR_SRV_ERROR_BRIDGE_CALL_FAILED;memcpy(output,&error,4);}
  return rc;
 }
