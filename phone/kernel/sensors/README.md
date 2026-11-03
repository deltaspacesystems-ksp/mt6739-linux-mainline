# MICROFW initial optical sensor modules

For measured DURA-L21 v62/#48 only. No built-in kernel changes. Copy sources/Makefile to D:/y5_linux_work/mfw-i2c1-probe and build with archive/pc_work/build_i2c1_probe.sh. Kernel tree must match the running image and have module symbols generated. Both native modules built W=1/-Werror.

Transport is a restricted diagnostic adapter, not a complete mainline MT6739 I2C driver. Default read-only, explicit enable=1; als_control=1 additionally permits only MAIN_CTRL0/2. Does not enable proximity/IR or touch regulators/clocks/pins. Fault latches prevent retry; pinned module requires reboot for recovery. Do not use generic i2cdetect/scans. The sensor driver is instantiated explicitly as mfw_ltr_c2 at0x53; no forced match to LTR501/F216A or automatic DT binding.

IIO exports raw light/channel0a only. ID0xc2 is measured; exact chip identity, sample freshness, lux calibration and physical light-response test remain unconfirmed. Rate0x22/gain1/CTRL0 checked at probe; each read enables ALS only, waits150ms and restoresCTRL0. No calibration claims.

Tests: generated exact-source host allowlist and IIO callback fault-injection in archive/pc_work, ASan/UBSan. The read/write allowlist covers1280+65536 combinations, including the i2c-dev DMA_SAFE annotation; it does not select hardware DMA. Explicit opt-in startup tools/mfw-light-start; no autostart.


## Physical tests and opt-in IR trial

2026-10-26 owner cover/lamp test with ALS continuously enabled45s: raw0..29897, restoredCTRL0. Short150ms on/off reads did not show a clear response; driver wait increased500ms but controlled per-read retest remains pending.

Optional module parameter ir_test=1 creates permission for explicit writing `run` to /sys/bus/i2c/devices/i2c-1/ir_trial (attribute exists with gateoff too, store refuses). Synchronous kernel callback holds bus, checksIDc2/CTRL0, savesreg0..3, applies exactstock4.4PS settings36/10/5c, enablesPS only for50samples100ms apart, disables and restores/readback-checks settings before returning. No process-lifetime dependency; nominal5s plus bounded I2C operations. Not an autostart or continuously running proximity driver. Defaultir_testfalse. After test gateoff reloaded. Public I2C_RDWR still cannot enableIR; extra read-only config registers1..3 allowed.

On a fault adapter latches/pins. A bounded internal PS-off command remains permitted even after latch; other retries/config writes remain blocked. Restoration failure reported, not masked. Exact callback tested67faultpositions plusnormal/busy/optin underASan/UBSan. Physicalfar14..23(median18), hand1-3cm182..268(median235.5), bothresult0/restored1. This confirms response, not calibratednear/far thresholds. NoIRQ/calibration/NVRAM operation.
