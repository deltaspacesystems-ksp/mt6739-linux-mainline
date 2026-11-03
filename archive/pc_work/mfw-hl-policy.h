/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_HL_POLICY_H
#define MFW_HL_POLICY_H
#define MFW_HL_BLOCK_TEMP 1U
#define MFW_HL_BLOCK_TIME 2U
#define MFW_HL_MAX_SESSION_S 43200UL /* stock DT max_charging_time */

/* Narrow bootstrap: 4.20 V, retaining a 100/500 mA USB input limit.
 * CE and TE belong to this manager; other settings must remain unchanged.
 */
static inline int mfw_hl_profile_supported(unsigned int control, unsigned int oreg,
		unsigned int ibat, unsigned int special, unsigned int safety)
{
	unsigned int base = control & ~12U;
	return (base == 0x30 || base == 0x70) && oreg == 0x8e &&
		(ibat & 0x7f) == 1 && (special & 0xe7) == 0x24 &&
		(safety == 0x40 || safety == 0x7b);
}

static inline unsigned int mfw_hl_blocks(int temp_valid, int temp_deci_c,
		unsigned long elapsed_s)
{
	unsigned int blocks = 0;
	/* Only the stock normal JEITA band; no low/high-temperature CV tiers. */
	if (!temp_valid || temp_deci_c < 100 || temp_deci_c >= 450)
		blocks |= MFW_HL_BLOCK_TEMP;
	if (elapsed_s >= MFW_HL_MAX_SESSION_S)
		blocks |= MFW_HL_BLOCK_TIME;
	return blocks;
}

static inline unsigned int mfw_hl_control(unsigned int old, unsigned int blocks)
{
	/* Restore observed LK termination-enable; control CE explicitly. */
	return ((old | 8) & ~4U) | (blocks ? 4 : 0);
}

static inline unsigned int mfw_hl_timer_value(unsigned int status)
{
	return (status & 0x40) | 0x80; /* preserve EN_STAT, strobe TMR_RST */
}
#endif
