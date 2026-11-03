#include <assert.h>
#include <stdio.h>
#include "y5-ntc.h"

int main(void)
{
    int t, previous = 601, valid = 0;
    assert(y5_ntc_deci_c(0, &t));
    assert(y5_ntc_deci_c(4095, &t));
    assert(y5_ntc_deci_c(4096, &t));
    assert(y5_ntc_deci_c(619, &t));
    assert(y5_ntc_deci_c(3771, &t));
    assert(!y5_ntc_deci_c(1523, &t) && t == 250); /* 10k NTC */
    assert(!y5_ntc_deci_c(1432, &t) && t == 277); /* measured on phone */
    assert(!y5_ntc_deci_c(620, &t) && t == 600);
    assert(!y5_ntc_deci_c(3770, &t) && t == -400);
    for (unsigned raw = 0; raw <= 4096; ++raw) {
        int ret = y5_ntc_deci_c(raw, &t);
        assert((ret == 0) == (raw >= 620 && raw <= 3770));
        if (!ret) {
            assert(t >= -400 && t <= 600 && t <= previous);
            previous = t;
            ++valid;
        }
    }
    printf("PASS: rails, table limits, known ADC values, monotonicity; %d valid codes\n", valid);
    return 0;
}
