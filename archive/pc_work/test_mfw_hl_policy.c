#include <assert.h>
#include <stdio.h>
#include "mfw-hl-policy.h"
int main(void)
{
    for (unsigned c=0;c<256;c++) {
        int ok=mfw_hl_profile_supported(c,0x8e,0x81,0x24,0x7b);
        assert(ok == (c==0x30 || c==0x34 || c==0x38 || c==0x3c ||
                      c==0x70 || c==0x74 || c==0x78 || c==0x7c));
        for (unsigned b=0;b<4;b++) {
            unsigned n=mfw_hl_control(c,b);
            assert(!((c^n)&~12U));
            assert(n&8);
            assert(!!(n&4)==!!b);
        }
        unsigned timer=mfw_hl_timer_value(c);
        assert(timer&0x80);
        assert(!(timer&~0xc0U));
        assert((timer&0x40)==(c&0x40));
    }
    assert(!mfw_hl_profile_supported(0x70,0xb2,0x81,0x24,0x7b));
    assert(!mfw_hl_profile_supported(0x70,0x8e,0xa1,0x24,0x7b));
    assert(mfw_hl_profile_supported(0x30,0x8e,0x81,0x24,0x40));
    assert(!mfw_hl_profile_supported(0xb0,0x8e,0x81,0x24,0x7b));
    assert(!mfw_hl_profile_supported(0xf0,0x8e,0x81,0x24,0x7b));
    assert(mfw_hl_blocks(1,99,0)==MFW_HL_BLOCK_TEMP);
    assert(!mfw_hl_blocks(1,100,43199));
    assert(!mfw_hl_blocks(1,449,0));
    assert(mfw_hl_blocks(1,450,0)==MFW_HL_BLOCK_TEMP);
    assert(mfw_hl_blocks(0,290,0)==MFW_HL_BLOCK_TEMP);
    assert(mfw_hl_blocks(1,290,43200)==MFW_HL_BLOCK_TIME);
    assert(mfw_hl_blocks(0,290,43200)==(MFW_HL_BLOCK_TEMP|MFW_HL_BLOCK_TIME));
    puts("PASS: profile guard, CE/TE-only updates, timer mask, NTC and 12h boundaries");
}
