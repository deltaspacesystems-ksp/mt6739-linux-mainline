/*
 * Copyright © 2022 Imagination Technologies Ltd.
 *
 * based in part on v3dv driver which is:
 * Copyright © 2019 Raspberry Pi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/* Offline probe: exact metadata allocators from pinned Mesa; no Vulkan/device initialization. */
#include "util/bitscan.h"
#include "hwdef/rogue_hw_defs.h"
static void allocate_var(pco_range *allocation_list,
                         unsigned *counter,
                         nir_variable *var,
                         unsigned dwords_each)
{
   unsigned slots = glsl_count_dword_slots(var->type, false);

   allocation_list[var->data.location] = (pco_range){
      .start = *counter,
      .count = slots * dwords_each,
   };

   *counter += slots * dwords_each;
}
static void try_allocate_var(pco_range *allocation_list,
                             unsigned *counter,
                             nir_shader *nir,
                             uint64_t bitset,
                             nir_variable_mode mode,
                             int location,
                             unsigned dwords_each)
{
   if (!(bitset & BITFIELD64_BIT(location)))
      return;

   nir_variable *var = nir_find_variable_with_location(nir, mode, location);
   assert(var);

   allocate_var(allocation_list, counter, var, dwords_each);
}
static void allocate_val(pco_range *allocation_list,
                         unsigned *counter,
                         unsigned location,
                         unsigned dwords_each)
{
   allocation_list[location] = (pco_range){
      .start = *counter,
      .count = dwords_each,
   };

   *counter += dwords_each;
}
static void try_allocate_vars(pco_range *allocation_list,
                              unsigned *counter,
                              nir_shader *nir,
                              uint64_t *bitset,
                              nir_variable_mode mode,
                              bool any,
                              bool f16,
                              enum glsl_interp_mode interp_mode,
                              unsigned dwords_each)
{
   uint64_t skipped = 0;

   while (*bitset) {
      int location = u_bit_scan64(bitset);
      unsigned accum = 0;

      nir_foreach_variable_with_modes (var, nir, mode) {
         if (var->data.location != location)
            continue;

         if (!any && (glsl_type_is_16bit(
                         glsl_without_array_or_matrix(var->type)) != f16 ||
                      var->data.interpolation != interp_mode)) {
            skipped |= BITFIELD64_BIT(location);
            break;
         }

         accum += glsl_count_dword_slots(var->type, false);
      }

      if (skipped & BITFIELD64_BIT(location))
         continue;

      accum *= dwords_each;
      allocate_val(allocation_list, counter, location, accum);
   }

   *bitset |= skipped;
}
static void check_unused_sysvals(
   BITSET_WORD system_values_read[static BITSET_WORDS(SYSTEM_VALUE_MAX)])
{
#if MESA_DEBUG
   if (!__bitset_is_empty(system_values_read, BITSET_WORDS(SYSTEM_VALUE_MAX))) {
      gl_system_value sysval;
      BITSET_FOREACH_SET (sysval, system_values_read, SYSTEM_VALUE_MAX) {
         debug_printf("unhandled sysval: %s\n", gl_system_value_name(sysval));
      }
   }
#endif
}
static void pvr_alloc_vs_sysvals(pco_data *data, nir_shader *nir)
{
   BITSET_DECLARE(system_values_read, SYSTEM_VALUE_MAX);
   BITSET_COPY(system_values_read, nir->info.system_values_read);

   gl_system_value sys_vals[] = {
      SYSTEM_VALUE_VERTEX_ID,     SYSTEM_VALUE_INSTANCE_ID,
      SYSTEM_VALUE_BASE_INSTANCE, SYSTEM_VALUE_BASE_VERTEX,
      SYSTEM_VALUE_DRAW_ID,       SYSTEM_VALUE_VIEW_INDEX,
   };

   for (unsigned u = 0; u < ARRAY_SIZE(sys_vals); ++u) {
      if (BITSET_TEST(system_values_read, sys_vals[u])) {
         nir_intrinsic_op op = nir_intrinsic_from_system_value(sys_vals[u]);
         unsigned dwords = nir_intrinsic_infos[op].dest_components;
         assert(dwords > 0);

         allocate_val(data->common.sys_vals,
                      &data->common.vtxins,
                      sys_vals[u],
                      dwords);

         BITSET_CLEAR(system_values_read, sys_vals[u]);
      }
   }

   check_unused_sysvals(system_values_read);
   assert(BITSET_IS_EMPTY(system_values_read));
}
static void pvr_alloc_vs_attribs(pco_data *data, nir_shader *nir)
{
   nir_foreach_shader_in_variable (var, nir) {
      allocate_var(data->vs.attribs, &data->common.vtxins, var, 1);
   }
}
static void pvr_alloc_vs_varyings(pco_data *data, nir_shader *nir)
{
   uint64_t vars_mask = nir->info.outputs_written &
                        BITFIELD64_RANGE(VARYING_SLOT_VAR0, MAX_VARYING);

   /* Varying ordering is specific. */
   try_allocate_var(data->vs.varyings,
                    &data->vs.vtxouts,
                    nir,
                    nir->info.outputs_written,
                    nir_var_shader_out,
                    VARYING_SLOT_POS,
                    1);

   /* Save varying counts. */
   nir_foreach_variable_with_modes (var, nir, nir_var_shader_out) {
      if (var->data.location < VARYING_SLOT_VAR0 ||
          var->data.location >= VARYING_SLOT_VAR0 + MAX_VARYING) {
         continue;
      }

      /* TODO: f16 support. */
      bool f16 = glsl_type_is_16bit(glsl_without_array_or_matrix(var->type));
      assert(!f16);
      unsigned components = glsl_get_components(var->type);

      switch (var->data.interpolation) {
      case INTERP_MODE_NONE:
         /* pco_rev_link_nir didn't run; override here. */
         var->data.interpolation = INTERP_MODE_SMOOTH;
         FALLTHROUGH;

      case INTERP_MODE_SMOOTH:
         if (f16)
            data->vs.f16_smooth += components;
         else
            data->vs.f32_smooth += components;

         break;

      case INTERP_MODE_FLAT:
         if (f16)
            data->vs.f16_flat += components;
         else
            data->vs.f32_flat += components;

         break;

      case INTERP_MODE_NOPERSPECTIVE:
         if (f16)
            data->vs.f16_npc += components;
         else
            data->vs.f32_npc += components;

         break;

      default:
         UNREACHABLE("");
      }
   }

   for (unsigned f16 = 0; f16 <= 1; ++f16) {
      for (enum glsl_interp_mode interp_mode = INTERP_MODE_SMOOTH;
           interp_mode <= INTERP_MODE_NOPERSPECTIVE;
           ++interp_mode) {
         try_allocate_vars(data->vs.varyings,
                           &data->vs.vtxouts,
                           nir,
                           &vars_mask,
                           nir_var_shader_out,
                           false,
                           f16,
                           interp_mode,
                           1);
      }
   }

   assert(!vars_mask);

   const gl_varying_slot last_slots[] = {
      VARYING_SLOT_PSIZ,       VARYING_SLOT_VIEWPORT,   VARYING_SLOT_LAYER,
      VARYING_SLOT_CLIP_DIST0, VARYING_SLOT_CLIP_DIST1,
   };

   for (unsigned u = 0; u < ARRAY_SIZE(last_slots); ++u) {
      try_allocate_var(data->vs.varyings,
                       &data->vs.vtxouts,
                       nir,
                       nir->info.outputs_written,
                       nir_var_shader_out,
                       last_slots[u],
                       1);
   }
}
static void pvr_alloc_fs_sysvals(pco_data *data, nir_shader *nir)
{
   BITSET_DECLARE(system_values_read, SYSTEM_VALUE_MAX);
   BITSET_COPY(system_values_read, nir->info.system_values_read);

   gl_system_value sys_vals[] = {
      SYSTEM_VALUE_FRONT_FACE,
   };

   for (unsigned u = 0; u < ARRAY_SIZE(sys_vals); ++u) {
      if (BITSET_TEST(system_values_read, sys_vals[u])) {
         nir_intrinsic_op op = nir_intrinsic_from_system_value(sys_vals[u]);
         unsigned dwords = nir_intrinsic_infos[op].dest_components;
         assert(dwords > 0);

         allocate_val(data->common.sys_vals,
                      &data->common.shareds,
                      sys_vals[u],
                      dwords);

         BITSET_CLEAR(system_values_read, sys_vals[u]);
      }
   }

   /* Clear built-in sysvals. */
   gl_system_value builtin_sys_vals[] = {
      SYSTEM_VALUE_SAMPLE_ID,
      SYSTEM_VALUE_LAYER_ID,
      SYSTEM_VALUE_SAMPLE_POS,
      SYSTEM_VALUE_BARYCENTRIC_LINEAR_CENTROID,
      SYSTEM_VALUE_BARYCENTRIC_LINEAR_COORD,
      SYSTEM_VALUE_BARYCENTRIC_LINEAR_PIXEL,
      SYSTEM_VALUE_BARYCENTRIC_LINEAR_SAMPLE,
      SYSTEM_VALUE_BARYCENTRIC_PERSP_CENTROID,
      SYSTEM_VALUE_BARYCENTRIC_PERSP_COORD,
      SYSTEM_VALUE_BARYCENTRIC_PERSP_PIXEL,
      SYSTEM_VALUE_BARYCENTRIC_PERSP_SAMPLE,
      SYSTEM_VALUE_BARYCENTRIC_PULL_MODEL,
   };

   for (unsigned u = 0; u < ARRAY_SIZE(builtin_sys_vals); ++u)
      BITSET_CLEAR(system_values_read, builtin_sys_vals[u]);

   check_unused_sysvals(system_values_read);
   assert(BITSET_IS_EMPTY(system_values_read));

   data->fs.meta = (pco_range){
      .start = data->common.shareds,
      .count = 1,
   };

   ++data->common.shareds;
}
static void pvr_alloc_fs_varyings(pco_data *data, nir_shader *nir)
{
   uint64_t vars_mask = nir->info.inputs_read &
                        BITFIELD64_RANGE(VARYING_SLOT_VAR0, MAX_VARYING);

   assert(!data->common.coeffs);

   /* Save the z/w locations. */
   unsigned zw_count = !!data->fs.uses.z + !!data->fs.uses.w;
   allocate_val(data->fs.varyings,
                &data->common.coeffs,
                VARYING_SLOT_POS,
                zw_count * ROGUE_USC_COEFFICIENT_SET_SIZE);

   /* If point coords are used, they come after z/w (if present). */
   nir_variable *var = nir_find_variable_with_location(nir,
                                                       nir_var_shader_in,
                                                       VARYING_SLOT_PNTC);
   if (var) {
      assert(!var->data.location_frac);
      unsigned count = glsl_get_components(var->type);
      assert(count == 2);

      allocate_var(data->fs.varyings,
                   &data->common.coeffs,
                   var,
                   ROGUE_USC_COEFFICIENT_SET_SIZE);

      data->fs.uses.pntc = true;
   }

   /* Allocate the rest of the input varyings. */
   try_allocate_vars(data->fs.varyings,
                     &data->common.coeffs,
                     nir,
                     &vars_mask,
                     nir_var_shader_in,
                     true,
                     false,
                     0,
                     ROGUE_USC_COEFFICIENT_SET_SIZE);
}
static void pvr_alloc_cs_sysvals(pco_data *data, nir_shader *nir)
{
   BITSET_DECLARE(system_values_read, SYSTEM_VALUE_MAX);
   BITSET_COPY(system_values_read, nir->info.system_values_read);

   gl_system_value vtxin_sys_vals[] = {
      SYSTEM_VALUE_LOCAL_INVOCATION_INDEX,
   };

   gl_system_value coeff_sys_vals[] = {
      SYSTEM_VALUE_WORKGROUP_ID,
      SYSTEM_VALUE_NUM_WORKGROUPS,
   };

   for (unsigned u = 0; u < ARRAY_SIZE(vtxin_sys_vals); ++u) {
      if (BITSET_TEST(system_values_read, vtxin_sys_vals[u])) {
         nir_intrinsic_op op =
            nir_intrinsic_from_system_value(vtxin_sys_vals[u]);
         unsigned dwords = nir_intrinsic_infos[op].dest_components;
         assert(dwords > 0);

         allocate_val(data->common.sys_vals,
                      &data->common.vtxins,
                      vtxin_sys_vals[u],
                      dwords);

         BITSET_CLEAR(system_values_read, vtxin_sys_vals[u]);
      }
   }

   for (unsigned u = 0; u < ARRAY_SIZE(coeff_sys_vals); ++u) {
      if (BITSET_TEST(system_values_read, coeff_sys_vals[u])) {
         nir_intrinsic_op op =
            nir_intrinsic_from_system_value(coeff_sys_vals[u]);
         unsigned dwords = nir_intrinsic_infos[op].dest_components;
         assert(dwords > 0);

         if (dwords > 1 && data->common.coeffs & 1)
            ++data->common.coeffs;

         allocate_val(data->common.sys_vals,
                      &data->common.coeffs,
                      coeff_sys_vals[u],
                      dwords);

         BITSET_CLEAR(system_values_read, coeff_sys_vals[u]);
      }
   }

   check_unused_sysvals(system_values_read);
   assert(BITSET_IS_EMPTY(system_values_read));
}
