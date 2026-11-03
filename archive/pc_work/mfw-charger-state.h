/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_CHARGER_STATE_H
#define MFW_CHARGER_STATE_H
enum mfw_charge_state { MFW_DISCHARGING, MFW_NOT_CHARGING, MFW_CHARGING, MFW_FULL };
/* HL7005D: STAT alone means PWM allowed, not necessarily enabled.
 * Also check CE, HZ_MODE and the CDIS pin status (EN_LEVEL).
 */
static inline enum mfw_charge_state mfw_charge_state(unsigned int status,
		unsigned int control, unsigned int special, int online)
{
	if (!online || (status & 8))
		return MFW_DISCHARGING;
	if (((status >> 4) & 3) == 2)
		return MFW_FULL;
	if (((status >> 4) & 3) == 1 && !(control & 6) && !(special & 8))
		return MFW_CHARGING;
	return MFW_NOT_CHARGING;
}
#endif
