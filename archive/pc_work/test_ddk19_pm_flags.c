/* SPDX-License-Identifier: MIT */
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
static bool mfw19_memory_flags(uint64_t flags, bool mapping, uint32_t *old)
{
 uint64_t mask=mapping?UINT64_C(0x0400070f):UINT64_C(0x84007fff);
 /* Only audited PM/FW protect device flag(bit26) is allowed; other hints rejected. */
 if(flags&~mask)return false;
 bool protect=(flags&UINT64_C(0x04000000))!=0;
 if(protect&&!mapping&&(flags&0x30))return false; /* No userspace CPU access to PM tables. */
 unsigned gpu=(flags>>8)&7,cpu=(flags>>11)&7;
 if(gpu!=1&&gpu!=3)return false;
 if(!mapping&&cpu!=0&&cpu!=3)return false;
 *old=(uint32_t)(flags & (mapping?0xf:UINT64_C(0x800040ff)));
 if(gpu==3)*old|=3u<<8; /* modern uncached1 -> old uncached0 */
 if(protect)*old|=UINT32_C(0x01000000); /* old DEVICE_FLAGS_OFFSET24, PMMETA_PROTECT0 */
 if(!mapping)*old|=(cpu==0?1u:3u)<<11; /* modern WC0 -> old WC1 */
 return true;
}
int main(void){uint32_t out=0;
assert(mfw19_memory_flags(UINT64_C(0x84004303),false,&out)&&out==UINT32_C(0x81004b03));
assert(mfw19_memory_flags(UINT64_C(0x04000303),true,&out)&&out==UINT32_C(0x01000303));
assert(mfw19_memory_flags(UINT64_C(0x80004333),false,&out)&&out==UINT32_C(0x80004b33));
assert(mfw19_memory_flags(UINT64_C(0x303),true,&out)&&out==UINT32_C(0x303));
for(unsigned cpu=1;cpu<=3;cpu++){out=123;assert(!mfw19_memory_flags(UINT64_C(0x84004303)|(cpu<<4),false,&out)&&out==123);}
assert(!mfw19_memory_flags(UINT64_C(0x88004303),false,&out));
assert(!mfw19_memory_flags(UINT64_C(0x08000303),true,&out));
assert(!mfw19_memory_flags(UINT64_C(0x81004303),false,&out));
assert(!mfw19_memory_flags(UINT64_C(0x01000303),true,&out));
for(unsigned bit=32;bit<64;bit++)assert(!mfw19_memory_flags(UINT64_C(0x84004303)|(UINT64_C(1)<<bit),false,&out));
puts("PASS PMprotect bit26->bit24, CPUpermission rejection, unknownflag/upperbit guards and unprotected regression");}
