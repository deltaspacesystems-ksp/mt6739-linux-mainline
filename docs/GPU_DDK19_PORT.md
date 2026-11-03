# Rogue DDK 1.9: isolated Linux 7.3 port

2026-10-25. Phone remains on verified v59/#44, GPU powered off. No boot writes,
restarts, firmware execution or vendor userspace installation in this step.

Public source: https://github.com/parthibx24/android_kernel_wiko_p200
Commit: `096323c2d5db8a05e84df31608fc0c9d22e47b31`.
Subtree: `drivers/misc/mediatek/gpu/gpu_rgx/m1.9ED4893595`.
All 590 source blobs verified against pinned Git blob SHA1, including two files
without final newline. Tree metadata and provenance: `phone/kernel/gpu/ddk19/`.
Keep per-file licenses: Imagination files inspected are Dual MIT/GPLv2; MTK files
can be GPLv2. Do not infer a blanket license from repository metadata.

The MT6739 config explicitly selects `22.49.21.16`, `rgx.fw.signed` and Android
ION/native sync. DDK version matches stock 1.9@4893595; this does not yet prove
complete firmware/userspace ABI compatibility. Vendor EGL still needs Bionic/HAL
dependencies. This path alone does not supply a native Alpine Mesa driver.

## Code and measured validation

`linux73-drm-initial.patch` is a four-file, compile-only overlay:

- Replace removed drmP.h with explicit modern DRM headers.
- Replace removed load/unload callbacks with explicit init/fini entry points,
  for future platform lifecycle integration.
- Use ddev->dev rather than removed platformdev; remove removed date/dev_priv_size
  and DRM_UNLOCKED. Retain bridge command ABI unchanged.
- Advertise render only and remove unused modeset initialization/cleanup.
- Use linux/stddef.h in kernel builds; fix external Kbuild include roots and
  remove paths absent from mainline. Remove obsolete declaration warning and
  replace vendor -Wno-error with -Werror.

Fresh ARM64 `pvr_drm.o`: PASS, W=1 and -Werror, no compiler warnings.
Exact-source replay -> patch application -> fresh object compile: PASS.
This is not a linked driver, usable render node, runtime lifecycle validation,
KMS implementation, zero-copy or GPU acceleration.

Other object attempts expose real port work:

- physmem_dmabuf: attach signature, removed kmap/kmap_atomic and iosys_map vmap.
  Port must preserve reservation locking, CPU access direction and map lifetimes;
  do not solve with casts or a fake kmap shim.
- pvr_sync2: missing Android staging sync.h; requires dma_fence/sync_file port.
- osfunc: removed linux/pfn_t.h, with further OS API changes still unassessed.

Next (initial milestone; see updated results below): port memory and native fences, replace MTK platform power with validated
CCF/SPM/regulator path, then link and audit firmware ABI. No blind loading of
vendor MTK power/DVFS/HWAPM code; no DT auto-binding yet.

## Reproduce on this PC

Reference source: `D:\y5_linux_work\sources\rogue-ddk19` (immutable).
Port: `D:\y5_linux_work\rogue-ddk19-port` (isolated).
Copy helpers from `archive/pc_work/` to `D:\y5_linux_work` if needed.
Run `python D:\y5_linux_work\replay_ddk19_port.py`.
It verifies all blobs, recopies original source files into the port directory,
applies the repo overlay, removes only its DRM object and recompiles that object.
Do not run replay over unexported port edits: export the overlay first.
`audit_ddk19_port.sh` attempts four objects independently; logs are
`D:\y5_linux_work\out\ddk19-*.log`. BLOCKED is an expected failed compile,
not a functioning component. No config, Image, DT, phone or installation changes.

## Update: dma-buf and native fences compile on 7.3

The cumulative `linux73-drm-initial.patch` now covers ten files, including the new
GPL helper `mfw-dmabuf-fill.h`. Fresh compilation of seven objects passes W=1 and
-Werror without warnings: DRM, physmem_dmabuf, pvr_sync_dma_fence,
pvr_counting_timeline, pvr_sw_fence, pvr_fence, dma_fence_sync_native_server.
Replay now rebuilds all seven and runs the helper tests.

Memory: modern attach signature; removed obsolete kmap exporter callbacks.
Imports use reservation-locking `_unlocked` map/unmap wrappers. Kernel mapping
retains an allocated iosys_map through release, validates bounds and rejects
IOMEM where PMR requires an ordinary pointer. Fill/poison supports iosys_map RAM
and IOMEM dispatch; CPU writes use matching DMA_TO_DEVICE begin/end. Failure
paths unmap/end/free correctly. DMA SG length uses sg_dma_len (coalesced DMA
segments), rejects empty/misaligned maps, and PMR creation errors unmap before
free. Vendor secure-buffer zero-SG behavior remains unsupported.

Fault injection on the exact fill helper passes ASan+UBSan: poison pattern resets
per page, partial final page, guard bytes, zero-fill, allocation failure, begin
failure, vmap failure/null map, end failure and first-error preservation. These
are mocked host RAM/IOMEM dispatch tests, not actual DMA/cache/GPU hardware tests.
PMR callback runtime and scatter-gather/IOMMU behavior still need integration tests.

Synchronization: DDK already contains a native dma_fence/sync_file backend; the
vendor Makefile selected Android staging sync instead. Select the existing native
backend and its five required objects, with SUPPORT_DMA_FENCE. Port access_ok,
strscpy calls (return values unused), and u64 fence seqno diagnostics. No fake
Android sync compatibility layer, no disabled native synchronization. This still
needs runtime fence lifetime/callback/rollback validation and userspace ABI audit.

Whole external build was refused because working kernel has CONFIG_MODULES=n;
explicit composite pvrsrvkm.o target also has no rule in this external built-in
setup. Do not change the live config merely to bypass this. Individual object
builds are valid but do not prove final linking. Next: remaining OS APIs, platform
lifecycle/power, ION replacement, full linkage and firmware/userspace compatibility.
Phone still v59; no writes, restart or GPU power request in this step.

## Update: native-only decision, full kernel-side archive/link

Owner explicitly requires the entire userspace stack to be native: no isolated
Bionic/HAL runtime either. Keep this decision across agent handoffs.

The initial overlay is now cumulative across shared headers and Linux OS APIs.
Full `KBUILD_BUILTIN=1 built-in.a` build succeeds without changing CONFIG_MODULES.
Relocatable ARM64 whole-archive link succeeds: text 586839, data 37856, bss 59356
bytes. After removing two optional MTK memory-stat hook assignments, every
undefined symbol is supplied by the current v59 vmlinux. This is stronger than
individual object compilation, but is not yet final Image linking or HW validation.
Builds now use an ext4 working copy `/home/y5/rogue-ddk19-port` for speed; D: stays
the source authority. `sync_ddk19_wsl.sh` transfers source only, without binaries.

Changes include raw PFN/mmap locks and vm_flags setters; explicit-protection
vmap/vunmap instead of the obsolete protected vm_map_ram API; dynamic shrinker
registration with allocation-failure cleanup and safe unregister ordering;
modern timers/raw monotonic clock; correctly typed debugfs callback; optional
GED logging replaced with pr_debug. CPU physical cache maintenance uses ARM64
arch_sync_dma_for_* rather than dereferencing potentially NULL dma_map_ops.
Fixed-field strncpy padding and strlcpy length-return behavior are preserved by
small native helpers. 147 bounded string cases pass ASan/UBSan, along with the
existing dma-buf fault-injection tests. PMR/runtime cache/sparse behavior remains
untested on the actual GPU.

Warning policy: W=1/-Werror, with narrowly scoped exceptions for
unused-but-set-variable, unused-const-variable (release/debug-only vendor data)
and suggest-attribute=format. API/type/format/cast/implicit-function errors still
fail builds. Existing MMU cleanup/debug switch fallthroughs are explicitly marked.
Android/ION, vendor HWAPM/DVFS and optional GPU trace/proc-print hooks are disabled;
GPU execution, memory accounting and native synchronization remain enabled.

Fence teardown uses a private reclaim-capable workqueue. Its finite retry chain
uses zero-delay requeue after bounded worker sleep, so drain_workqueue sees the
whole chain; no flushing global system workqueues. Runtime fence completion and
device lifetime still require hardware tests.

New native platform binding is `microfw,mt6739-rogue-ddk19`, built-in only, disabled
unless its `enable` parameter is explicitly set. No matching node is on the phone.
Native power callbacks reuse the v59 bounded MFG sequence, fixed 26 MHz and
read-only VCORE 1.05..1.3125 V. Failed transitions latch fault and retain the clock;
uncertain teardown pins context until reboot. No firmware/GPU core register writes
in the power helper. This new integration has compile/link evidence only, not a
runtime power/firmware result. Before future binding disable the old ID probe DT
node to prevent concurrent ownership. Stock GPU DT evidence gives register range
0x13000000/0x80000 and SPI226 LEVEL_LOW; hardware IRQ delivery is still untested.

### Native Mesa route and ABI evidence

Upstream Mesa has a fully open pvrsrvkm winsys using direct DRM ioctls. Reference:
https://chromium.googlesource.com/external/gitlab.freedesktop.org/mesa/mesa/+/2736dd1220d58192f5591ca6f9fcc5e60ee368ad/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv.c
Five reference files are pinned and Git-blob verified by
`fetch_mesa_srv_reference.py`; source stays under D:/y5_linux_work/sources/.
This backend requires DDK1.17@6256262. Our 1.9@4893595 requires a real bridge port;
do not fake the version. Hardware-specific Mesa support/quirks/compiler behavior
for 22.49.21.16 also remain work, not a proven rendering route.

CONNECT input is 16 bytes in both ABIs, but 1.9 order is flags/options/version/build;
1.17 is options/build/version/flags. Output is 17 bytes in both ABIs: 1.9 has
arch@0, caps@1, PVR bridges@5, RGX bridges@9, error@13; 1.17 has BVNC@0, error@8,
caps@12, arch@16. A raw structure reuse would corrupt interpretation.

`phone/kernel/gpu/ddk19/native_probe.c` is a native C client using the original
generated 1.9 headers and exact static ABI assertions. Host build/--abi and static
AArch64 cross-build pass. Copied only to phone RAM /tmp and executed successfully:
DDK1.9@4893595, input16/output17/error-offset13/options80000010. Negative test
`--connect /dev/null` returns1; default --abi returns0. No GPU node opened, no
firmware or GPU power request. Explicit --connect refuses driver name/version/build
mismatches and sends genuine 1.9 CONNECT/DISCONNECT, never submits rendering jobs.
No vendor userspace library is linked. The static executable is currently in RAM;
the phone remains on v59, no restart/boot write.


### 2026-10-26: mfw_v60 native GPU Image prepared, no boot write
Integrated595 DDk source files into Linux7.3 with dedicated boolean Kconfig.
Fullkernel compile found3old-style definitions missed in isolated build; changed
PMRInit/PMRDeInit/PVRSRVGetPVRSRVData to(void). Image#45 built withnohup/ARM64cross.
Image13719560B, boot9089024B, SHA256ab33cc6a26370fd8512d5bf700886fb15bf5f5af42d4fbf4c09d7e4f97334092.
Config delta onlyDRM_MFW_ROGUE_DDK19=y; initramfs/cmdline/addressesidenticalv59.
DT19 addsnativeGPUreg13000000/80000,SPI226/low,validatedCCF/VCORE/PMICreferences;
oldIDprobedisabled topreventdualownership. NootherDTchanges. Defaultnativebinding
disabled. Fullselfcontained627filekernelpatch9830162B dryrunpristine7.3PASS.
Backupboot24MiB+RAMstagingv60PASS; no write/reboot yet. Phone stillv59/#44.
Importantcodeaudit: CommonDeviceInit onlysetsbridges/sync; firmwareinitialization
happensonFIRSTrendernodeOPEN (CommonDeviceOpen). Thusnative_probe --connect is
anexplicitfirmware/power test evenbeforeversionioctl, notreadonlyquery. Prepared
script onlystagesFW(readonlyvendor->tmpfs) andbinds; inspectlogs thennativeCopen.
NoBionic/HAL,shaderjobs yet. ActualFWboot/IRQ/renderingremainunverified.
Preservedpre-DDKv59kernel-symbolbaselineonD/out, SHA2563d5faac8c9cd4590ad108212b9d838dea796f88f06c4522d4d09174d46da3bcb,
andcheckscriptrefusesbaselinecontainingDDK topreventself-resolvingsymbolmistakes.
Firmwaretrialscriptgatesbuild#45/noGUI/nooldID/VCORE,onlyvendorp41ro,noload,
fixedfirmwarefilename/hash/size, tmpfsstage, singleattemptmarker. Built-inonly,
suppressedunbind; faultclock/contextremainpinneduntilreboot. Needexplicitowner
bootwrite/restartconsent under originalrule9; runtimeFWtestrequiresownerPoweronLK.


### 2026-10-26: v60 boot/bind confirmed; v61 fixes measured DRM-open blocker
Owner explicitly approvedv60 write/reboot/nativeFWtest. Bootp36write+fullimageSHA
readbackPASS; ownerPower/startconfirmed. uname#45,defaultenableN,VCOREselector101
1150000uV/enabled1,Charging. Readonlyvendorp41 firmware copied onlytotmpfs with
size1265664/SHAverified; vendorunmounted. ExplicitnativebindPASS: card1/renderD128,
LinuxGPUirq17(fromstockSPI226low), compiletimeBVNC22.49.21.16. This is notnewHWID.
FirstnativeC--connect: openEINVAL. KernelWARN drm_file.c:329 beforedriveropen:
new7.3core requires filp->f_op->fop_flags & FOP_UNSIGNED_OFFSET. No firmware startup
in this attempt; do not describebindRESULT0 as firmware/accelerationPASS.
VCORE/Chargingunchanged. BroadinitialGPUlogfilter includedbootcmdline; narrowed
toexcludeKernelcommandline, sanitizedownedRAMlog. Neverexportdeviceidentifiers.
V61solefunctionalchange addsmandatoryFOP_UNSIGNED_OFFSET. Image#46,13719560B,
boot9089024B,SHA2561970fa060e0be018dc43f72c141940f31088be70520466de0d1722bd3e2d3f13.
DT19/config/initramfs/cmdline/addressesidenticalv60; validatorPASS; deltaapply/repro
exactcompiledsourcePASS. v61patchisdeltaONv60, notanotherfullpatch. Backup24MiB
andRAMstagingPASS; no v61write/reboot yet. Phonev60. Needownerconsent(rule9).
NativeCclient extendedwith genuine1.9heapqueries (IDs28/30, not1.17IDs30/32),
generatedstruct/staticasserts. Host+AArch64buildPASS; ASan/UBSanmockbridge8casesPASS:
wirefields,versionrefusal,countbounds,unterminatednames,IOerrors,disconnectcleanup.
Realheapquerynotrun becausefirstopenblocked. NewclientcurrentlyinphoneRAM.
PrimaryMediaTekMT6739specnamesGE8100(up570MHz), notGE8300. Runtimeidentityremains
actualBVNC22.49.21.16. DDKconfig22features/5errata/3enhancements extractedasreference,
only14Mesafieldsdirectlymapped. NotcompleteMesaGPUprofile: unknownUSC/tile/compiler
valuesmustnotdefaultfalseorcopyGE8300. Sourceandconfidencegpu-ge8100-feature-reference.json.


### 2026-10-26: offline native Mesa ABI audit while v61 approval pending
Mapped42 static Mesa1.17 bridge constants against DDK1.9:9namesmissing,7numericIDs
changed. EqualIDs donotmean equalwirelayouts; runtimecapabilitymasksstillrequired.
Results phone/hardware/mesa-ddk19-bridge-command-map.json; nativeheapclientuses
generated1.9layouts and hostmock tests, notunchecked1.17structs.
OnlystaticELFsymbolmetadata read from genericvendorlib64libusc/libsrv_um/
libglslcompiler (p41ro,noload,unmountedafter). FoundPVRUniFlex/PVRSRVsymbolnames;
readelfwarnedaboutdynsymsh_info; symbolreferencesnotassumedallexports. No binaries
copied or executed, noBionic/HALruntime, no newhardwareprofilederived. This is
referenceanalysisonly, consistentwithowner'snative-onlyrequirement.
Phone stillv60; v61prepared/staged, approvalrequestpending. No additionalGPUopen,
powerrequest,partitionwriteor restart during theseoffline/readonlyaudits.


### 2026-10-26: v61 native firmware/CONNECT/heap queries confirmed
OwnerPower/startconfirmed. uname#46,initialenableN,VCORE1150000uV/selector101/enabled1,
Charging. Repeatedguardedbindwithreadonlyvendor->tmpfsfirmwarepreparationPASS;
nativecard1/renderD128. FirstnativeCOPEN/CONNECT/DISCONNECTnowPASS:error0,arch64,
caps00000100,pvrbridges0008a847,rgxbridges00001447. Firmwarebasenamefallbacknormal:
twoBVNCsuffixcandidatesENOENT, thenpinnedgenericrgx.fw.signedloaded. Debugfsstatus:
DriverOK,FirmwareOK,HWR0,CRR0,APM0,GPUutil0%. IRQ17/MT_SYSIRQ226count1.
Native--heapsPASS17GPUVAheaps: General65c0000000/80000000, PDSa800000000/40000000,
USCba00000000/40000000, VisTeste400000000/100000, plusotherdriver-definedheaps.
Driverdebugversionreports22.49.21.16(HW); bindlogusedcompiledfallback, so do not
callthisanindependentrawIDread. ActualrawIDwasverifiedv59. NativeCPUstackonly:
noBionic/HAL/GPUvendorCPUlibraryexecution. Firmwareboot+IRQ+serviceAPIconfirmed,
NOTshader/renderingacceleration/zero-copy/video. NextnativeRAM/VMAPItestsandMesa
bridge/profileport; no need to restart for userland changes.
OWNERNEWCONSTRAINT: nowawayfromphone; NO further phone restarts. If GPUacceleration
blockedbynewkernel/restartrequirements, continue modem/WiFi/BT/otherhardware safe
work andsourcepreparation. No protectedpartition/NVRAMwrites, no nvram_daemon.


## 2026-10-26 — v61 native PMR CPU mapping, no reboot
Measured on v61/#46: native C client allocates a zeroed locked 4096-byte PMR, maps via the opaque handle, verifies all 1024 words, unmaps and frees successfully. Repeated at uptime 6h59; Driver/Firmware OK, HWR=0, CRR=0, utilisation=0%. This proves CPU mapping, not GPU execution or acceleration. Exact generated DDK1.9 ABI uses 32-bit allocation flags, 52-byte allocation input and 12-byte output; do not copy 1.17 structures. Host mock suite: 13 scenarios including allocation/mmap/initial-zero/free failures, ASan+UBSan PASS. Owner away: no reboot/reset/suspend or boot writes; continue native userspace and offline radio work.


## 2026-10-26 — full Mesa build and initial DDK1.9 adapter
Pinned completeMesa2736dd1220d58192f5591ca6f9fcc5e60ee368ad fetched toWSL/home/y5/mesa-native. FixedLLVM23 CLC APIs and NIR kernel-export handling; beforefix generatedhelperprogram enumempty despite14SPIR-Ventrypoints. Six exactpass regressions ASan/UBSanPASS; fullx86_64PowerVR Vulkanlibrarybaseline+experimentalDDK19 buildsPASS. ReferenceGX6250 helpergeneration only, noGE8100profile substitution.
ActualMesa Mesonopt imagination-mfw-ddk19 adds exactCONNECT/DISCONNECT1.9translation+driver/buildguards, skipsobsoleteINIT, rejectsalluntranslatedbridgecallsENOTSUP. FixedBVNC is explicitDURA-L21buildtarget, notnewquery. Adaptertests ASan/UBSanPASS; two patches replaypristinesourceexact7files. No phoneMesa installed, no rendering/shader claim. Remainingmemory/sync/jobABIandcompleteGE8100profile. phone/mesa/README.md and phone/hardware/mesa-stage1-validation.json.
GenericstockUSCcompiler copied onlytoD/private_gpu_reference forstaticanalysis, size/SHAverified, neverexecuted/committed. .dynsym28exports/nofullsymtab, targetBVNCpatternsabsent. StaticfunctionGetTileBufferSize returns0x3000(12288), verifiedARM64instructions; notequivalenttoprovenhardwareUSCcommonstore. Vendorunmountedverified. NoBionic/HAL. WSLbuildpackagesinstalledexactsignedversions viaArcharchiveaftermirror404; nofullupgrade/Windowschanges. Phone staysv62, GPUFWhealthy/Charging; no further restart. User wants persistence andlowtokenusage; ifGPUdata/ABIblocks, proceedWiFi/BT/modemwithoutasking.
