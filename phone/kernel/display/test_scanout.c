/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-ovl-scanout.h"
static int plan(unsigned layer,u64 addr,u64 size,struct mfw_ovl_scanout *p)
{
 return mfw_ovl_prepare_scanout(layer,addr,size,720,1440,2880,MFW_XRGB8888,p);
}
int main(void)
{
 struct mfw_ovl_scanout p;
 const u64 bytes=4147200;
 assert(!plan(3,0x775eb000,bytes,&p));
 assert(p.address_offset==0xfa0 && p.pitch_offset==0xa4 && p.size_offset==0x98);
 assert(p.address==0x775eb000 && p.pitch==2880 && p.size==0x05a002d0 && p.required_bytes==bytes);
 assert(plan(3,0x775e8e38,bytes,&p)==-EINVAL);
 assert(plan(4,0x60000000,bytes,&p)==-EINVAL);
 assert(plan(0,0,bytes,&p)==-EINVAL);
 assert(plan(0,0x60000000,bytes-1,&p)==-EINVAL);
 assert(plan(0,0x100000000ULL,bytes,&p)==-ERANGE);
 assert(plan(0,0xfffff000ULL,bytes,&p)==-ERANGE);
 assert(!plan(0,0xffc0b000ULL,bytes,&p));
 assert(plan(0,0x60000000,bytes,NULL)==-EINVAL);
 assert(mfw_ovl_prepare_scanout(0,0x60000000,bytes,721,1440,2880,MFW_XRGB8888,&p)==-EINVAL);
 assert(mfw_ovl_prepare_scanout(0,0x60000000,bytes,720,1441,2880,MFW_XRGB8888,&p)==-EINVAL);
 assert(mfw_ovl_prepare_scanout(0,0x60000000,bytes,720,1440,2944,MFW_XRGB8888,&p)==-EINVAL);
 assert(mfw_ovl_prepare_scanout(0,0x60000000,bytes,720,1440,2880,0,&p)==-EINVAL);
 for(unsigned layer=0;layer<4;layer++){
  assert(!plan(layer,0x60000000,bytes,&p));
  assert(p.address_offset==0xf40+layer*0x20);
 }
 puts("PASS: measured OVL3 layout; DMA32 end bounds, size, pitch, alignment, format and layer validation (UBSan)");
 return 0;
}
