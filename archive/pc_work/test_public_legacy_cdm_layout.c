/*************************************************************************/ /*!
@File
@Title          RGX firmware interface structures
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    RGX firmware interface structures used by srvclient
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
typedef uint64_t IMG_UINT64;typedef uint32_t IMG_UINT32;
#define RGXFW_ALIGN __attribute__((aligned(8)))
typedef struct _RGXFWIF_CDM_REGISTERS_
{
	IMG_UINT64	uCDMReg_CDM_CTRL_STREAM_BASE;
	IMG_UINT64	uCDMReg_TPU_BORDER_COLOUR_TABLE;
	IMG_UINT64	uCDMReg_COMPUTE_THREAD_BARRIER;


	IMG_UINT64  uCDMReg_PDS_CTRL;

	IMG_UINT64  uCDMReg_TPU_YUV_CSC_COEFFICIENTS;

#if defined(RGX_FEATURE_COMPUTE_MORTON_CAPABLE)
	IMG_UINT64	uCDMReg_CDM_ITEM;
#endif
#if defined(RGX_FEATURE_TPU_CEM_DATAMASTER_GLOBAL_REGISTERS)
	IMG_UINT64  uCDMReg_TPU_CEM_CDM;
#else
	IMG_UINT64  uCDMReg_TPU;
#endif
	IMG_UINT64  uCDMReg_TPU_ARRAYS_CDM;
#if defined(RGX_FEATURE_CLUSTER_GROUPING)
	IMG_UINT64  uCDMReg_COMPUTE_CLUSTER;
#endif

#if defined(RGX_FEATURE_TPU_DM_GLOBAL_REGISTERS)
	IMG_UINT64 uCDMReg_TPU_TAG_CDM_CTRL;
#endif
} RGXFWIF_CDM_REGISTERS;
typedef struct _RGXFWIF_CDM_REGISTERS_CSWITCH_
{
	IMG_UINT64	uCDMReg_CDM_CONTEXT_STATE_BASE_ADDR;
	IMG_UINT64	uCDMReg_CDM_CONTEXT_PDS0;
	IMG_UINT64	uCDMReg_CDM_CONTEXT_PDS1;
	IMG_UINT64  uCDMReg_CDM_TERMINATE_PDS;
	IMG_UINT64  uCDMReg_CDM_TERMINATE_PDS1;
#if defined(RGX_SW_COMPUTE_PDS_BARRIER) && (RGX_NUM_PHANTOMS == 2)
	IMG_UINT64  uCDMReg_CDM_TERMINATE_PDS_PH2;	/* PDS task variant which synchronises 2 phantoms. Not used in FW */
	IMG_UINT64  uCDMReg_CDM_TERMINATE_PDS1_PH2;
#endif

	/* CDM resume controls */
	IMG_UINT64	uCDMReg_CDM_RESUME_PDS0;
	IMG_UINT64	uCDMReg_CDM_RESUME_PDS1;
#if defined(RGX_FEATURE_COMPUTE_MORTON_CAPABLE)
	IMG_UINT64	uCDMReg_CDM_CONTEXT_PDS0_B;
	IMG_UINT64	uCDMReg_CDM_RESUME_PDS0_B;
#endif

} RGXFWIF_CDM_REGISTERS_CSWITCH;
typedef struct _RGXFWIF_CMD_COMPUTE_
{
	RGXFWIF_CDM_REGISTERS			RGXFW_ALIGN sCDMRegs;	/*!< CDM registers */
	RGXFWIF_CDM_REGISTERS_CSWITCH	RGXFW_ALIGN sCDMRegsCSwitch;/*!< CDM registers for ctx switch */
#if defined(FIX_HW_BRN_54441)
	RGXFWIF_CDM_54441_REGISTERS		RGXFW_ALIGN	sBRN54441CDMRegs;
#endif

	IMG_UINT32				ui32Flags;	/*!< Control flags */
	IMG_UINT32				ui32MaxDeadlineMS; /*!< Max HWR deadline limit in ms */
} RGXFWIF_CMD_COMPUTE;

/* Feature-neutral candidate only: do not infer unknown target features absent. */
_Static_assert(sizeof(RGXFWIF_CDM_REGISTERS)==56,"runtime regs");
_Static_assert(sizeof(RGXFWIF_CDM_REGISTERS_CSWITCH)==56,"context regs");
_Static_assert(sizeof(RGXFWIF_CMD_COMPUTE)==120,"command");
_Static_assert(offsetof(RGXFWIF_CMD_COMPUTE,sCDMRegsCSwitch)==56,"context boundary");
_Static_assert(offsetof(RGXFWIF_CMD_COMPUTE,ui32Flags)==112,"flags");
_Static_assert(offsetof(RGXFWIF_CMD_COMPUTE,ui32MaxDeadlineMS)==116,"deadline");
#define CHECK(field,expected) do{assert(offsetof(RGXFWIF_CDM_REGISTERS,field)==expected);printf("%u %s\n",expected,#field);}while(0)
int main(void){
CHECK(uCDMReg_CDM_CTRL_STREAM_BASE,0);
CHECK(uCDMReg_TPU_BORDER_COLOUR_TABLE,8);
CHECK(uCDMReg_COMPUTE_THREAD_BARRIER,16);
CHECK(uCDMReg_PDS_CTRL,24);
CHECK(uCDMReg_TPU_YUV_CSC_COEFFICIENTS,32);
CHECK(uCDMReg_TPU,40);
CHECK(uCDMReg_TPU_ARRAYS_CDM,48);
puts("PASS public historical feature-neutral120B layout matches observed boundaries; NOT exact-target layout proof");
}
