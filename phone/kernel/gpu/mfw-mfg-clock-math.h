/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_MFG_CLOCK_MATH_H
#define MFW_MFG_CLOCK_MATH_H
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif
/* MT6739 PLL21bit PCW,7integerbits ->14fractional; divider CON1[26:24].
 * This decodes registers, not a measured frequency counter result. */
static inline u64 mfw_mfg_pll_rate(u32 con0,u32 con1,u32 power){
 if(!(con0&1)||!(power&1)||(power&2))return 0;
 return ((26000000ULL*(con1&0x1fffff))>>14)>>((con1>>24)&7);
}
#endif
