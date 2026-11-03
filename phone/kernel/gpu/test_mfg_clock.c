/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-mfg-clock-math.h"
int main(void){
 u64 rate=mfw_mfg_pll_rate(0x181,0x0215ec4e,0x80000001);
 assert(rate>=569999000&&rate<=570001000);
 assert(!mfw_mfg_pll_rate(0,0x0215ec4e,1));
 assert(!mfw_mfg_pll_rate(1,0x0215ec4e,0));
 assert(!mfw_mfg_pll_rate(1,0x0215ec4e,3));
 assert(mfw_mfg_pll_rate(1,1U<<14,1)==26000000);
 assert(mfw_mfg_pll_rate(1,(7U<<24)|(1U<<14),1)==203125);
 printf("PASS: MT6739 PLL decode/live register candidate=%lluHz, enable/isolation and divider boundaries\n",(unsigned long long)rate);return 0;
}
