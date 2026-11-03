/* SPDX-License-Identifier: GPL-2.0-only */
/* MT6739 topology/registers derived from stock get_cur_phy_freq(). */
#ifndef MFW_CPU_RATE_H
#define MFW_CPU_RATE_H
static inline unsigned long long mfw_cpu_rate(unsigned int pll, unsigned int div,
		unsigned int mux, unsigned long reference)
{
	unsigned int num = 1, den = 1, post = (pll >> 24) & 7;
	unsigned long long rate;
	if (((mux >> 8) & 15) != 1 || post > 4 || !reference)
		return 0; /* Only the verified ARMPLL path is implemented. */
	switch (div & 31) {
	case 0: /* LK leaves CLKDIV=0; stock default is undivided. */
	case 8: break;
	case 9: num = 3; den = 4; break;
	case 10: den = 2; break;
	case 11: den = 4; break;
	case 17: num = 4; den = 5; break;
	case 18: num = 3; den = 5; break;
	case 19: num = 2; den = 5; break;
	case 20: den = 5; break;
	case 25: num = 5; den = 6; break;
	case 26: num = 4; den = 6; break;
	case 27: num = 3; den = 6; break;
	case 28: num = 2; den = 6; break;
	case 29: den = 6; break;
	default: return 0;
	}
	rate = (unsigned long long)(pll & 0x3fffff) * reference;
	return (rate * num) / ((1ULL << (14 + post)) * den);
}
#endif
