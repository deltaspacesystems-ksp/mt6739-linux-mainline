#include "pco/pco.h"
#include "pco/pco_data.h"
#include "common/pvr_device_info.h"
#include "util/ralloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "pco_metadata_allocators.h"
/* Offline restricted-path experiment: incomplete profile MUST NOT be installed. */
int main(int argc,char **argv){
 unsigned depth=argc>=2?(unsigned)strtoul(argv[1],NULL,10):0;
 unsigned slots=argc>=3?(unsigned)strtoul(argv[2],NULL,10):16;
 /* Offline-only: avoid unsigned underflow/zero capacity in pinned helper.
  * banks8, line128 registers/instance, reserved2 lines. Not target limits. */
 if(depth>256||slots<1||slots>64||(depth&&((depth*8u)/128u)<=2u))return 2;
 struct pvr_device_info dev={.ident={.b=22,.v=49,.n=21,.c=16},.features={.has_num_clusters=true,.num_clusters=1,.has_roguexe=true,.has_num_raster_pipes=true,.num_raster_pipes=1,.has_simple_internal_parameter_format=true,.has_simple_parameter_format_version=true,.simple_parameter_format_version=1,.has_simple_internal_parameter_format_v1=true,.has_pbe2_in_xe=true,.has_compute=true,.has_gs_rta_support=true,.has_slc_cache_line_size_bits=true,.slc_cache_line_size_bits=512}};
 if(depth){dev.features.has_unified_store_depth=true;dev.features.unified_store_depth=depth;dev.features.has_usc_slots=true;dev.features.usc_slots=slots;} /* SYNTHETIC SENSITIVITY INPUT ONLY */
 void *mem=ralloc_context(NULL);pco_ctx*ctx=pco_ctx_create(&dev,mem);
 unsigned stage=argc>=4?(unsigned)strtoul(argv[3],NULL,10):0;
 if(stage>2)return 2;
 nir_builder b=nir_builder_init_simple_shader(stage==1?MESA_SHADER_VERTEX:stage==2?MESA_SHADER_FRAGMENT:MESA_SHADER_COMPUTE,pco_nir_options(),"mfw_offline_probe");
 b.shader->info.internal=false;
 pco_data data={0};
 if(stage==0){
  b.shader->info.workgroup_size[0]=1;b.shader->info.workgroup_size[1]=1;b.shader->info.workgroup_size[2]=1;
  nir_store_global_2x32(&b,nir_imm_int(&b,0x12345678),nir_imm_ivec2(&b,0xc0000000u,0x65u),.write_mask=1,.align_mul=4);
 }else if(stage==1){
  nir_variable *pos=nir_variable_create(b.shader,nir_var_shader_out,glsl_vec4_type(),"gl_Position");pos->data.location=VARYING_SLOT_POS;
  nir_def *id=nir_load_vertex_id(&b);
  nir_def *x=nir_bcsel(&b,nir_ieq_imm(&b,id,0),nir_imm_float(&b,-.5),nir_bcsel(&b,nir_ieq_imm(&b,id,1),nir_imm_float(&b,.5),nir_imm_float(&b,0)));
  nir_def *y=nir_bcsel(&b,nir_ieq_imm(&b,id,2),nir_imm_float(&b,.5),nir_imm_float(&b,-.5));
  nir_store_var(&b,pos,nir_vec4(&b,x,y,nir_imm_float(&b,0),nir_imm_float(&b,1)),15);


 }else{
  nir_variable *color=nir_variable_create(b.shader,nir_var_shader_out,glsl_vec4_type(),"color");color->data.location=FRAG_RESULT_DATA0;
  nir_store_var(&b,color,nir_imm_vec4(&b,.2,.6,1,1),15);
  data.fs.outputs[FRAG_RESULT_DATA0]=(pco_range){.start=0,.count=4};data.fs.output_formats[FRAG_RESULT_DATA0]=PIPE_FORMAT_R8G8B8A8_UNORM;data.fs.rasterization_samples=1;data.fs.z_replicate=~0u;
 }
 pco_preprocess_nir(ctx,b.shader);pco_lower_nir(ctx,b.shader,&data);pco_postprocess_nir(ctx,b.shader,&data);
 if(stage==0){
 unsigned stores=0,invocation_loads=0;
 nir_foreach_function_impl(impl,b.shader){nir_foreach_block(block,impl){nir_foreach_instr(instr,block){
  if(instr->type==nir_instr_type_tex)return 2;
  if(instr->type==nir_instr_type_intrinsic){
   nir_intrinsic_instr *intr=nir_instr_as_intrinsic(instr);
   if(intr->intrinsic==nir_intrinsic_store_global_2x32)stores++;
   else if(intr->intrinsic==nir_intrinsic_load_local_invocation_index)invocation_loads++;
   else {fprintf(stderr,"Unexpected compute intrinsic %s\n",nir_intrinsic_infos[intr->intrinsic].name);return 2;}
  }
 }}}
 if(stores!=1||invocation_loads!=1)return 2;
 puts("OFFLINE_NIR_GUARD_PASS one invocation-index load, one global store; no texture/interpolation/barrier intrinsic");
 }
 FILE *nir_out=fopen("/tmp/mfw-pco-restricted.nir","w");if(!nir_out)return 2;
 nir_print_shader(b.shader,nir_out);if(fclose(nir_out))return 2;
 if(stage==1){pvr_alloc_vs_sysvals(&data,b.shader);pvr_alloc_vs_attribs(&data,b.shader);pvr_alloc_vs_varyings(&data,b.shader);}
 else if(stage==2){pvr_alloc_fs_sysvals(&data,b.shader);pvr_alloc_fs_varyings(&data,b.shader);}
 else pvr_alloc_cs_sysvals(&data,b.shader);
 pco_shader *shader=pco_trans_nir(ctx,b.shader,&data,mem);pco_process_ir(ctx,shader);
 FILE *ir_out=fopen("/tmp/mfw-pco-restricted.ir","w");if(!ir_out)return 2;
 pco_print_shader(shader,ir_out,"offline final grouped IR");if(fclose(ir_out))return 2;
 pco_encode_ir(ctx,shader);
 const pco_data *final=pco_shader_data(shader);
 if(stage==1){printf("VS_CONTRACT outputs=%u vertex_id=%u:%u entry=%u coeffs=%u\n",final->vs.vtxouts,final->common.sys_vals[SYSTEM_VALUE_VERTEX_ID].start,final->common.sys_vals[SYSTEM_VALUE_VERTEX_ID].count,final->common.entry_offset,final->common.coeffs);}
 if(stage==2){printf("FS_CONTRACT phase=%u w=%u z=%u feedback=%u discard=%u early=%u sample_shading=%u coeffs=%u meta=%u:%u output=%u:%u entry=%u\n",final->fs.uses.phase_change,final->fs.uses.w,final->fs.uses.z,final->fs.uses.depth_feedback,final->fs.uses.discard,final->fs.uses.early_frag,final->fs.uses.sample_shading,final->common.coeffs,final->fs.meta.start,final->fs.meta.count,final->fs.outputs[FRAG_RESULT_DATA0].start,final->fs.outputs[FRAG_RESULT_DATA0].count,final->common.entry_offset);}
 unsigned size=pco_shader_binary_size(shader);
 pco_precomp_data meta=pco_get_precomp_data(shader);
 printf("OFFLINE_META temps=%u vtxins=%u shareds=%u\n",meta.temps,meta.vtxins,meta.shareds);
 FILE *out=fopen("/tmp/mfw-pco-restricted.bin","wb");if(!out)return 2;
 if(fwrite(pco_shader_binary_data(shader),1,size,out)!=size||fclose(out))return 2;
 printf("OFFLINE_ONLY restricted shader encoded bytes=%u; incomplete profile, NOT hardware validated\n",size);
 pco_print_binary(shader,stdout,"MICROFW offline restricted-path experiment");
 ralloc_free(b.shader);ralloc_free(mem);return size?0:1;
}
