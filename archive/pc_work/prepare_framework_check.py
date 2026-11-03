from pathlib import Path
r=Path('/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19')
s=(r/'services/include/rgx_fwif_resetframework.h').read_text().replace('#include "rgx_fwif_shared.h"','#define RGXFW_ALIGN __attribute__ ((aligned (8))) /* exact Linux definition in rgx_common.h */')
Path('/mnt/d/y5_linux_work/out/rgx_fwif_resetframework_check.h').write_text(s)
p=Path('/mnt/d/y5_linux_work/transfer_abi_assert.c');s=p.read_text().replace('#include "rgx_fwif_resetframework.h"','#include "/mnt/d/y5_linux_work/out/rgx_fwif_resetframework_check.h"');p.write_text(s)
