/* SPDX-License-Identifier: GPL-2.0-only */
/* Stock DURA-L21 kernel table, 0xffffff80094b57f8. */
#ifndef Y5_NTC_H
#define Y5_NTC_H
static inline int y5_ntc_deci_c(unsigned int raw, int *temp)
{
	static const struct { int deci_c; unsigned int ohm; } ntc[] = {
		{-400, 195652}, {-350, 148171}, {-300, 113347}, {-250, 87559},
		{-200, 68237}, {-150, 53650}, {-100, 42506}, {-50, 33892},
		{0, 27219}, {50, 22021}, {100, 17926}, {150, 14674},
		{200, 12081}, {250, 10000}, {300, 8315}, {350, 6948},
		{400, 5834}, {450, 4917}, {500, 4161}, {550, 3535}, {600, 3014}
	};
	unsigned int r, i;

	/* 1.8 V reference and 16.9 kohm pull-up, no BIF series resistor.
	 * Reject rails and readings outside the verified table, never clamp an
	 * open/short sensor to a plausible charge-control temperature.
	 */
	if (!raw || raw >= 4095)
		return -1;
	r = raw * 16900 / (4096 - raw);
	if (r > ntc[0].ohm || r < ntc[20].ohm)
		return -1;
	for (i = 1; i < 21; i++) {
		if (r >= ntc[i].ohm) {
			unsigned int span = ntc[i-1].ohm - ntc[i].ohm;
			*temp = ntc[i-1].deci_c + (int)
				(((ntc[i-1].ohm - r) * 50 + span / 2) / span);
			return 0;
		}
	}
	return -1;
}
#endif
