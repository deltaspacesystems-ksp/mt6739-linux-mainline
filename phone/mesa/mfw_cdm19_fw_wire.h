/* SPDX-License-Identifier: MIT */
/* Wire layout independently recovered from exact firmware DWARF. No device operations. */
#ifndef MFW_CDM19_FW_WIRE_H
#define MFW_CDM19_FW_WIRE_H
#include <stdint.h>
#include <stddef.h>
struct mfw_cdm19_fw_regs {
 uint64_t uCDMReg_CDM_CTRL_STREAM_BASE;
 uint64_t uCDMReg_TPU_BORDER_COLOUR_TABLE;
 uint64_t uCDMReg_COMPUTE_THREAD_BARRIER;
 uint64_t uCDMReg_PDS_CTRL;
 uint64_t uCDMReg_TPU_YUV_CSC_COEFFICIENTS;
 uint64_t uCDMReg_TPU;
 uint64_t uCDMReg_TPU_ARRAYS_CDM;
};
_Static_assert(sizeof(struct mfw_cdm19_fw_regs)==56,"exact firmware size");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_CDM_CTRL_STREAM_BASE)==0,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_TPU_BORDER_COLOUR_TABLE)==8,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_COMPUTE_THREAD_BARRIER)==16,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_PDS_CTRL)==24,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_TPU_YUV_CSC_COEFFICIENTS)==32,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_TPU)==40,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_regs,uCDMReg_TPU_ARRAYS_CDM)==48,"exact firmware member offset");
struct mfw_cdm19_fw_ctx {
 uint64_t uCDMReg_CDM_CONTEXT_STATE_BASE_ADDR;
 uint64_t uCDMReg_CDM_CONTEXT_PDS0;
 uint64_t uCDMReg_CDM_CONTEXT_PDS1;
 uint64_t uCDMReg_CDM_TERMINATE_PDS;
 uint64_t uCDMReg_CDM_TERMINATE_PDS1;
 uint64_t uCDMReg_CDM_RESUME_PDS0;
 uint64_t uCDMReg_CDM_RESUME_PDS1;
};
_Static_assert(sizeof(struct mfw_cdm19_fw_ctx)==56,"exact firmware size");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_CONTEXT_STATE_BASE_ADDR)==0,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_CONTEXT_PDS0)==8,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_CONTEXT_PDS1)==16,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_TERMINATE_PDS)==24,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_TERMINATE_PDS1)==32,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_RESUME_PDS0)==40,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_ctx,uCDMReg_CDM_RESUME_PDS1)==48,"exact firmware member offset");
struct mfw_cdm19_fw_cmd {
 struct mfw_cdm19_fw_regs sCDMRegs;
 struct mfw_cdm19_fw_ctx sCDMRegsCSwitch;
 uint32_t ui32Flags;
 uint32_t ui32MaxDeadlineMS;
};
_Static_assert(sizeof(struct mfw_cdm19_fw_cmd)==120,"exact firmware size");
_Static_assert(offsetof(struct mfw_cdm19_fw_cmd,sCDMRegs)==0,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_cmd,sCDMRegsCSwitch)==56,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_cmd,ui32Flags)==112,"exact firmware member offset");
_Static_assert(offsetof(struct mfw_cdm19_fw_cmd,ui32MaxDeadlineMS)==116,"exact firmware member offset");
#endif
