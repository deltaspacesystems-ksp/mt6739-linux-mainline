"""Wire native platform lifetime/power. Binding is disabled unless explicitly enabled."""
from pathlib import Path
import re
base = Path('D:/y5_linux_work')
root = base / 'rogue-ddk19-port'
p = root / 'kernel/drivers/staging/imgtec/pvr_platform_drv.c'
original = (base / 'sources/rogue-ddk19' / p.relative_to(root)).read_text()
license_text = original[:original.index('#include')]
p.write_text(license_text + '''#include <drm/drm_device.h>
#include <drm/drm_drv.h>
#include <drm/drm_print.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include "module_common.h"
#include "pvr_drv.h"

/* Future explicit bringup only; no GPU activation on a default boot. */
static bool enable;
module_param(enable, bool, 0600);
MODULE_PARM_DESC(enable, "Explicitly permit native MICROFW GPU initialization");

static int pvr_probe(struct platform_device *pdev)
{
\tstruct drm_device *ddev;
\tint ret;
\tif (!enable)
\t\treturn dev_err_probe(&pdev->dev, -ENODEV, "Native GPU binding disabled\\n");
\tddev = drm_dev_alloc(&pvr_drm_generic_driver, &pdev->dev);
\tif (IS_ERR(ddev))
\t\treturn PTR_ERR(ddev);
\tret = pvr_drm_device_init(ddev);
\tif (ret)
\t\tgoto put;
\tret = drm_dev_register(ddev, 0);
\tif (!ret)
\t\treturn 0;
\tpvr_drm_device_fini(ddev);
put:
\tdev_set_drvdata(&pdev->dev, NULL);
\tdrm_dev_put(ddev);
\treturn ret;
}

static void pvr_remove(struct platform_device *pdev)
{
\tstruct drm_device *ddev = platform_get_drvdata(pdev);
\tdrm_dev_unplug(ddev);
\tpvr_drm_device_fini(ddev);
\tplatform_set_drvdata(pdev, NULL);
\tdrm_dev_put(ddev);
}
static void pvr_shutdown(struct platform_device *pdev)
{
\tstruct drm_device *ddev = platform_get_drvdata(pdev);
\tif (ddev)
\t\tPVRSRVCommonDeviceShutdown(ddev->dev_private);
}
static const struct of_device_id pvr_of_ids[] = {
\t{ .compatible = "microfw,mt6739-rogue-ddk19" }, { }
};
MODULE_DEVICE_TABLE(of, pvr_of_ids);
static struct platform_driver pvr_platform_driver = {
\t.driver = {
\t\t.name = "microfw-pvrsrvkm",
\t\t.of_match_table = pvr_of_ids,
\t\t.pm = &pvr_pm_ops,
\t\t.suppress_bind_attrs = true,
\t},
\t.probe = pvr_probe, .remove = pvr_remove, .shutdown = pvr_shutdown,
};
static int __init pvr_init(void)
{
\tint ret = PVRSRVCommonDriverInit();
\tif (ret)
\t\treturn ret;
\tret = platform_driver_register(&pvr_platform_driver);
\tif (ret)
\t\tPVRSRVCommonDriverDeinit();
\treturn ret;
}
late_initcall(pvr_init);
''' , newline='\n')
p = root / 'services/server/env/linux/module_common.c'
s = p.read_text().replace('#include "mtk_mfgsys.h"', '')
s = s.replace('#include "pvr_drv.h"', '#include "pvr_drv.h"\n#include <drm/drm_file.h>')
s = s.replace('\tMTKMFGSystemInit();', '\t/* Device-scoped native MFG setup happens in SysDevInit. */')
s = s.replace('\tMTKMFGSystemDeInit();', '\t/* SysDevDeInit owns native MFG teardown. */')
p.write_text(s, newline='\n')
p = root / 'services/system/rgx_mtk/sysconfig.c'
s = (base / 'sources/rogue-ddk19' / p.relative_to(root)).read_text()
s = s.replace('#include "mtk_mfgsys.h"', '#include "mfw-ddk-power.h"\n#include <linux/err.h>')
s = s.replace('PVRSRV_ERROR err = PVRSRV_OK;', 'PVRSRV_ERROR err = PVRSRV_OK;\n\tstruct mfw_ddk_power *power;')
s = s.replace('gsRGXTimingInfo.ui32CoreClockSpeed = RGX_HW_CORE_CLOCK_SPEED;', 'gsRGXTimingInfo.ui32CoreClockSpeed = 26000000;')
start = s.index('#if MTK_PM_SUPPORT')
end = s.index('\t/* Setup RGX specific data */', start)
s = s[:start] + '\tgsRGXTimingInfo.bEnableActivePM = IMG_FALSE;\n\tgsRGXTimingInfo.bEnableRDPowIsland = IMG_FALSE;\n\n' + s[end:]
s = s.replace('\t\tstruct resource *irq_res;', '\t\tint irq;')
s = s.replace('\t\tirq_res = platform_get_resource(gpsPVRCfgDev, IORESOURCE_IRQ, 0);', '\t\tirq = platform_get_irq(gpsPVRCfgDev, 0);')
s = s.replace('if (irq_res)', 'if (irq >= 0)').replace('irq_res->start', 'irq')
s = s.replace('gsDevices[0].pfnPrePowerState = MTKDevPrePowerState;', 'gsDevices[0].pfnPrePowerState = mfw_ddk_pre_power;')
s = s.replace('gsDevices[0].pfnPostPowerState = MTKDevPostPowerState;', 'gsDevices[0].pfnPostPowerState = mfw_ddk_post_power;')
s = s.replace('\tMTKRGXDeviceInit(gsDevices);', '''\tpower = mfw_ddk_power_create(pvOSDevice);
\tif (IS_ERR(power)) {
\t\tgsDevices[0].pvOSDevice = NULL;
\t\treturn PTR_ERR(power) == -EPROBE_DEFER ? PVRSRV_ERROR_PROBE_DEFER : PVRSRV_ERROR_INIT_FAILURE;
\t}
\tgsDevices[0].hSysData = power;''')
s = s.replace('\tMTKRGXDeviceDeInit(gsDevices);', '\tmfw_ddk_power_destroy(psDevConfig->hSysData);\n\tpsDevConfig->hSysData = NULL;')
p.write_text(s, newline='\n')
p = root / 'services/server/common/pvr_notifier.c'
s = p.read_text().replace('\tif (bQuiet == IMG_TRUE)\n\t\tg_use_id = MTKPP_ID_SHOT_FW;\n\telse\n\t\tg_use_id = MTKPP_ID_FW;', '#if defined(MTK_DEBUG_PROC_PRINT)\n\tif (bQuiet == IMG_TRUE)\n\t\tg_use_id = MTKPP_ID_SHOT_FW;\n\telse\n\t\tg_use_id = MTKPP_ID_FW;\n#endif')
p.write_text(s, newline='\n')
p = root / 'services/server/devices/rgx/rgxsrvinit.c'
s = p.read_text().replace('OSStringLength(RGX_FW_FILENAME)+MAX_BVNC_STRING_LEN', 'sizeof(RGX_FW_FILENAME)-1+MAX_BVNC_STRING_LEN')
p.write_text(s, newline='\n')
p = root / 'Makefile'
s = p.read_text().replace('services/system/rgx_mtk/$(MTK_PLATFORM)/mtk_mfgsys.o', 'services/system/rgx_mtk/mfw-ddk-power.o')
p.write_text(s, newline='\n')
