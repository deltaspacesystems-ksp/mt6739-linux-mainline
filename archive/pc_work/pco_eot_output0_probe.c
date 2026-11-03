#include "pco/pco.h"
#include "pco/pco_data.h"
#include "common/pvr_device_info.h"
#include "util/ralloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "pco_metadata_allocators.h"
/* Offline restricted-path experiment: incomplete profile MUST NOT be installed. */
int main(void){
 unsigned depth=0;
 unsigned slots=1;
 /* Offline-only: avoid unsigned underflow/zero capacity in pinned helper.
  * banks8, line128 registers/instance, reserved2 lines. Not target limits. */
 if(depth>256||slots<1||slots>64||(depth&&((depth*8u)/128u)<=2u))return 2;
 struct pvr_device_info dev={.ident={.b=22,.v=49,.n=21,.c=16},.features={.has_num_clusters=true,.num_clusters=1,.has_roguexe=true,.has_num_raster_pipes=true,.num_raster_pipes=1,.has_simple_internal_parameter_format=true,.has_simple_parameter_format_version=true,.simple_parameter_format_version=1,.has_simple_internal_parameter_format_v1=true,.has_pbe2_in_xe=true,.has_compute=true,.has_gs_rta_support=true,.has_slc_cache_line_size_bits=true,.slc_cache_line_size_bits=512}};
 if(depth){dev.features.has_unified_store_depth=true;dev.features.unified_store_depth=depth;dev.features.has_usc_slots=true;dev.features.usc_slots=slots;} /* SYNTHETIC SENSITIVITY INPUT ONLY */
 void *mem=ralloc_context(NULL);pco_ctx*ctx=pco_ctx_create(&dev,mem);

 nir_builder b=nir_builder_init_simple_shader(MESA_SHADER_FRAGMENT,pco_nir_options(),"mfw_eot_candidate");
 nir_intrinsic_instr *emit=nir_emitpix_pco(&b,nir_imm_int(&b,0x70000000),nir_imm_int(&b,0x00082019));
 nir_intrinsic_set_freep(emit,true);nir_jump(&b,nir_jump_return);
 pco_data data={.fs.uses.olchk_skip=true};
 pco_preprocess_nir(ctx,b.shader);pco_lower_nir(ctx,b.shader,&data);pco_postprocess_nir(ctx,b.shader,&data);
 pco_shader *shader=pco_trans_nir(ctx,b.shader,&data,mem);pco_process_ir(ctx,shader);pco_encode_ir(ctx,shader);
 pco_precomp_data meta=pco_get_precomp_data(shader);
 printf("EOT_CANDIDATE temps=%u vtxins=%u shareds=%u bytes=%u; offline only\n",meta.temps,meta.vtxins,meta.shareds,pco_shader_binary_size(shader));
 FILE *out=fopen("/mnt/d/y5_linux_work/out/graphics-eot-output0.bin","wb");assert(out);unsigned size=pco_shader_binary_size(shader);assert(fwrite(pco_shader_binary_data(shader),1,size,out)==size);assert(!fclose(out));
 FILE *ir=fopen("/mnt/d/y5_linux_work/out/graphics-eot-output0.ir","w");assert(ir);pco_print_shader(shader,ir,"EOT candidate");assert(!fclose(ir));
 ralloc_free(mem);return 0;
}
