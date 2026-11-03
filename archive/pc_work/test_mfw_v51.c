#include <assert.h>
#include <stdio.h>
#include "mfw-cpu-rate.h"
#include "mfw-charger-state.h"
int main(void)
{
    /* 26 MHz * PCW / 2^14 / 2 = 1.3 GHz. Fractional divider cases. */
    assert(mfw_cpu_rate(0x01190000, 0, 0x100, 26000000) == 1300000000ULL);
    assert(mfw_cpu_rate(0x01190000, 9, 0x100, 26000000) == 975000000ULL);
    assert(mfw_cpu_rate(0x01190000, 10, 0x100, 26000000) == 650000000ULL);
    assert(mfw_cpu_rate(0x01190000, 17, 0x100, 26000000) == 1040000000ULL);
    assert(mfw_cpu_rate(0x01190000, 25, 0x100, 26000000) == 1083333333ULL);
    assert(mfw_cpu_rate(0x811244ec, 0, 0x100, 26000000) == 949999755ULL);
    assert(!mfw_cpu_rate(0x01190000, 31, 0x100, 26000000));
    assert(!mfw_cpu_rate(0x07190000, 8, 0x100, 26000000));
    assert(!mfw_cpu_rate(0x01190000, 8, 0, 26000000));
    /* Exhaust every combination of status/control/special and online/boost.
     * A disabled charger or absent source must never report charging.
     */
    for (unsigned s=0;s<256;s++) for (unsigned c=0;c<256;c++)
        for (unsigned e=0;e<16;e++) {
            assert(mfw_charge_state(s,c,e,0) == MFW_DISCHARGING);
            enum mfw_charge_state state=mfw_charge_state(s,c,e,1);
            if ((s & 8)) assert(state == MFW_DISCHARGING);
            if (state == MFW_CHARGING)
                assert(((s>>4)&3)==1 && !(c&6) && !(e&8) && !(s&8));
        }
    assert(mfw_charge_state(0x10,0,0,1) == MFW_CHARGING);
    assert(mfw_charge_state(0x10,4,0,1) == MFW_NOT_CHARGING);
    assert(mfw_charge_state(0x10,0,8,1) == MFW_NOT_CHARGING);
    assert(mfw_charge_state(0x20,0,0,1) == MFW_FULL);
    assert(mfw_charge_state(0x36,0,0,1) == MFW_NOT_CHARGING);
    puts("PASS: CPU clock math and exhaustive charger disable/offline checks");
}
