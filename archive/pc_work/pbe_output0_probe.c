/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "pvr_job_common.h"
#include "csbgen/rogue/pbestate.h"
#include "vk_format.h"
int main(void){
 struct pvr_device_info dev={0};
 struct pvr_pbe_surf_params s={.swizzle={PIPE_SWIZZLE_X,PIPE_SWIZZLE_Y,PIPE_SWIZZLE_Z,PIPE_SWIZZLE_W},.is_normalized=true,.pbe_packmode=ROGUE_PBESTATE_PACKMODE_U8U8U8U8,.source_format=ROGUE_PBESTATE_SOURCE_FORMAT_8_PER_CHANNEL,.nr_components=4,.addr={.addr=UINT64_C(0x65c0000000)},.mem_layout=PVR_MEMLAYOUT_LINEAR,.stride=64,.depth=1,.width=64,.height=64};
 struct pvr_pbe_render_params p={.max_x_clip=63,.max_y_clip=63};uint32_t cs[4]={0xdeadbeef,0,0,0xdeadbeef};uint64_t regs[5]={0xdeadbeef,0,0,0,0xdeadbeef};
 pvr_arch_pbe_pack_state(&dev,&s,&p,cs+1,regs+1);
 assert(cs[0]==0xdeadbeef&&cs[3]==0xdeadbeef&&regs[0]==0xdeadbeef&&regs[4]==0xdeadbeef);
 FILE *out=fopen("/mnt/d/y5_linux_work/out/graphics-pbe-output0.bin","wb");assert(out);assert(fwrite(cs+1,4,2,out)==2);assert(fwrite(regs+1,8,3,out)==3);assert(!fclose(out));
 printf("PBE_RGBA8_LINEAR64_CANDIDATE_PASS state=%08x %08x regs=%016llx %016llx %016llx; offline only\n",cs[1],cs[2],(unsigned long long)regs[1],(unsigned long long)regs[2],(unsigned long long)regs[3]);
}

