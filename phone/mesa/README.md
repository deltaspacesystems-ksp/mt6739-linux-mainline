# Mesa native port, stage1

Pinned source: https://chromium.googlesource.com/external/gitlab.freedesktop.org/mesa/mesa/+/2736dd1220d58192f5591ca6f9fcc5e60ee368ad/ . Checkout /home/y5/mesa-native in WSL archlinux, fetched by exact commit without configuring a remote. No changes to X:/y5linux remotes.

Apply mesa-llvm23-build.patch then mesa-ddk19-stage1.patch. Both replay from pristine pinned files and match seven edited files byte-for-byte. Full host x86_64 Vulkan PowerVR library builds; this is NOT a phone binary and is not installed on the phone. Configured headless/services, no Gallium/GL/EGL. Build uses reference GX6250 precompiled-helper selection only as a build check; this does not assign GX6250/GE8300 capabilities to GE8100 or demonstrate working shaders there.

Build fixes: LLVM23 moved GetResourcesPath and changed lookupTarget to Triple; NIR export fix retains unique-named OpenCL entrypoint wrappers (new Clang inner names __clang_ocl_kern_imp_*), while retaining the leading-underscore hiding rule. Before the NIR fix SPIR-V had14entrypoints but generated shader enumeration was empty. Exact pass tested in six old/new/helper/private/duplicate cases under ASan/UBSan; full shader generation/library link now passes.

Experimental Meson option imagination-mfw-ddk19 defaults false. When true, fixed-platform 1.9 build/options and exact CONNECT16/17byte translation with version/name/build guards are used; DISCONNECT receives the required4byte input. Old errors are translated only as success/generic failure, not numerically assumed compatible. BVNC is explicitly the measured DURA-L21 build target22.49.21.16 because old CONNECT carries noBVNC; it is NOT a fresh driver query and a complete device profile is still absent. Init ioctl is skipped only for services, which1.9 initializes on OPEN. All untranslated generic bridge calls fail ENOTSUP, so unchecked1.17 layouts are never sent as1.9. This remains a partial adapter, not a rendering driver. Host mock ABI/error/refusal tests pass using actual Mesa struct declarations.

Reproduction scripts are in archive/pc_work. Source patches are authoritative, not the non-idempotent development-edit scripts. Meson/ninja/Python build dependencies and LLVM23/Clang/libclc/translator installed in WSL only. A stale mirror had404for some exact package versions; same-version signed packages/signatures obtained from archive.archlinux.org and pacman-U verified them. No whole-system upgrade and no Windows driver/network changes. Build logs D:/y5_linux_work/out/mesa-*.log.

## Remaining work

Translate memory/heaps/contexts/reservations/map flags, then sync and render/compute/transfer submission, against generated1.9 ABI. Modern reserved heap-carveout and fence semantics require explicit treatment. Populate GE8100 profile only from evidence; unknown features cannot become false by default. Then native Alpine/aarch64 build and first offscreen shader with readback.

Static USC reference: generic vendor/lib64/libusc.so copied only to private PC analysis folder D:/y5_linux_work/private_gpu_reference, verified ELF size/SHA256, never executed or committed. Vendor mounted ro,noload and unmounted afterward. Public ELF exports28functions and no full symbols/debug information. Exact target BVNC pattern scan found none. Static instructions in PVRUniFlexGetTileBufferSize return12288; this is not sufficient to infer USC common-store/tile geometry. Metadata in phone/hardware/usc-static-profile-reference.json, raw binary/decompilation remain private. Large telnet transfer did not produce a recognized completion marker; byte size/SHA256/ELF checks independently passed and vendor unmount verified separately. No Bionic/HAL/runtime use.


## Stage2 — context and heap ABI

Apply mesa-ddk19-stage2-on-stage1.patch AFTER stage1. Adds context create/destroy, heap count (1.17command30 ->1.9command28, reversed return-field order), heap create/destroy (reordered input fields). Context return cache-line/error order translated explicitly. Null/size/kernel-context/4K-heap guards. Generated1.9 compiler check confirms RETRY25 and BRIDGE_CALL_FAILED37; preserve RETRY, map other unknown errors generically. Host ASan/UBSan tests and full library build PASS; patch replay byte-exact. Native standalone phone smoke uses this exact adapter and actual DRM ioctls, not mocked hardware or the full x86_64 Mesa library: context/heap create/count/destroy plus CONNECT/DISCONNECT PASS, 17heaps/cacheline64B/FWOK/HWR0/CRR0. Phone lacks libdrm development headers, so smoke uses a minimal checked64-byte DRM version UAPI wrapper. No render job. Seven operations supported; heap details/reservations/PMRs/sync/jobs still fail closed.

## Stage3a — GPU virtual-address reservation

Apply mesa-ddk19-stage3-on-stage2.patch after stage2. Adds RESERVERANGE/UNRESERVERANGE (nine supported operations total). Translates modern address,size,heap into exact1.9 heap,address,size; packed24-byte input/12-byte output. Rejects null heap, zero/unaligned length or address, overflowing range and incorrect sizes before ioctl; preserves RETRY for cleanup. Generated tests use current Mesa structs and inspect old wire bytes; ASan/UBSan PASS. Full host Vulkan library links. Native standalone phone test uses same adapter with real DRM ioctls: CONNECT/context/17heaps/heap/reserve4096/unreserve/destroy/DISCONNECT PASS; Driver/FirmwareOK,HWR0,CRR0. No restart or rendering job. Patch replay byte-exact.

Next: explicit memory allocation/cache-flag translation (modern64-bit vs old32-bit with different cache-mode values), PMR allocation/free and GPU map/unmap; then synchronization and submission. Do not truncate flags or infer modern reserved heap regions absent from old ABI. Complete target compiler profile remains unresolved.

## Stage3b — PMR allocation / GPU mapping

Apply mesa-ddk19-stage3b-on-stage3a.patch after stage3a. Four more operations (13 total): PMR allocate/free and GPU map/unmap. Exact1.9 packed52-byte allocation and28-byte mapping input,12-byte handle/error output. Reorders fields and explicitly translates cache encodings: modern GPU uncached1 -> old0, modern CPU WC0 -> old1; incoherent3 preserved. Only ordinary single-chunk 4K buffers, zero pid/pdump and verified permission/kernel-mappable/zero flags accepted. Sparse/SVM/device/heap hints and unverified flags reject before ioctl. CPU/GPU cache combinations checked across all64 pairs; high bits never truncated. Old ABI has no out_flags; zero means unavailable here, not a measured effective flag set. Pinned Mesa wrapper does not consume that field. Nonzero pid remains unsupported, so this is still a partial adapter.

Host sanitizer tests and full library link PASS, exact patch replay PASS. Native standalone exact-adapter phone test: allocate4096, verify zero, CPU pattern write/read, reserve/mapGPU/unmap/unreserve/free/destroy/disconnect PASS. Driver/FirmwareOK,HWR0,CRR0. No reboot, shader, render job or GPU readback; CPU readback alone does not prove GPU cache coherence. Next heap details/compatibility gaps, synchronization and submission plus target compiler profile.

## Stage4a — sync block lifecycle

Apply mesa-ddk19-stage4-on-stage3b.patch after stage3b. Adds sync ALLOC/SET/FREE (16operations total). Allocation sends required4-byte placeholder and reorders28-byte legacy output into modern handle/pmr/error/size/address. SET16-byte handle/index/value layout matches; FREE8-byte input matches. Errors translated, RETRY preserved. Old export handle is PMR_LOCAL_EXPORT_HANDLE, a child of the sync block: do not feed it to ordinary PMR mmap/free. No local-import translation yet.

Host ASan/UBSan wire/guards/errors and full library link PASS, patch replay exact. Native standalone exact-adapter phone test alloc4096/set0x12345678/clear0/free PASS; PMR/map lifecycle stillPASS; FirmwareOK/HWR0/CRR0. This proves API return success, not independent memory readback or GPU completion/fence signaling. No render job/reboot.

Remaining compatibility gaps confirmed in actual pinned source: buffer creation passes getpid() while conservative allocation adapter currently accepts only0; heap details includes modern reserved_size with no direct1.9 equivalent. Neither gap is papered over. Next inspect exact legacy local-import and allocation accounting semantics, then implement/test appropriate translation before job submission.

## Sync memory readback diagnostic

Standalone phone smoke now explicitly traverses old MM3(make process import handle from sync export child), MM6(local-import PMR), read-only mmap; then munmap/MM7(unref imported PMR)/MM4(unmake handle), before SYNC1(parent free). These are legacy-only diagnostic commands, not new generic modern adapter cases; supported modern operations remain16. Source/type checks: generated server_mm_bridge distinguishes PMR_LOCAL_EXPORT_HANDLE, process DEVMEM_MEM_IMPORT and connection PHYSMEM_PMR; imported PMR release is7, not allocation unlock/free8.

Phone readback of0x12345678 set via translated SYNC2 PASS on4096-byte imported buffer; full teardown/PMR regressionPASS, FirmwareOK/HWR0/CRR0. Exact helper ASan/UBSan success and7 injected failures (make/import/mmap/value/munmap/unref/unmake) PASS; parent release stops on teardown errors. This is CPU readback of kernel-set sync memory, not GPU completion. No restart or render job. Evidence mesa-sync-readback-validation.json.

## Current-process PID compatibility

Apply mesa-ddk19-current-pid-on-stage4.patch after stage4. Ordinary PMR now accepts pid0 or getpid(), matching pinned buffer_create caller; foreign PID remains rejected because1.9 has no explicit allocation PID field. No modern PID field is sent into old wire. Host current/foreign-PID tests and adapter regression ASan/UBSan PASS, full library link and exact patch replayPASS. Phone current-pid allocation/CPU pattern/GPU mapping and sync import readback12345678/cleanupPASS; FirmwareOK/HWR0/CRR0. No reboot/render.

Confirmed heap blocker: pvr_srv.c heap-init passes returned carveout_size into pvr_winsys_heap_init; old details returns base,length,page,import-alignment,tiling but no reserved_size. Need evidence-based static-data/carveout policy, not arbitraryzero. Earlier zero-pid-only restrictions are superseded by this section.

## Transfer-context diagnostic (after current-PID patch)

Apply mesa-ddk19-transfer-context-on-current-pid.patch. Adds restricted bridge128 CREATE0/DESTROY1,18supported operations total. Legacy input24/output12 bytes; modern extra CLI/USC handles explicitlyNULL (not complete modern semantic equivalence). Only priority0/default CCB/no context flags/robustness, allzero16-byte reset-framework accepted. Its size/disabled flag checked from original generated/firmware declarations with isolated header dependencies; no register commands or reset-framework execution enabled.

ASan/UBSan exact wire/guards/errors, full host link, exact patch replay PASS. Phone native standalone exact-adapter contextcreate/destroy with bounded RETRY teardown PASS; PMR/map/sync-readback regressionPASS, Driver/FirmwareOK,HWR0,CRR0. No GPU job or rendering/reboot.

Critical submission difference: modern SUBMITTRANSFER2 is command3, but legacy3 is SETTRANSFERCONTEXTPRIORITY! Legacy submit is command2 and has another layout (fence/server-sync arrays and one update timeline vs separate modern2D/3D). Do not forward unchecked. Firmware transfer payload and target features need independent verification before submitting a task.

## Context-program source correction
Before regenerating USC context programs apply mesa-context-sr-last-burst.patch on pinned Mesa. It prevents unsigned underflow in the final partialDMA chunk. CPU scheduling oracle1539legal casesPASS and SPIRV rebuildPASS; this is not GPU execution validation. Standalone restricted context compiler is archive/pc_work/build_pco_context20.py (requires generated pco_ra_budget8.c from build_pco_budget8.py). Explicit budget20 is a software constraint, not hardware profile. See gpu-native-context-usc-candidates.json.

## PM/FW protection (after transfer-context patch)
Apply mesa-ddk19-pm-protect.patch to translate exactly moderndevicebit26 to legacybit24 forallocation/map; otherdevicehints remain rejected. Protectedbuffers cannotrequest userCPUread/write permissions. Kernel-onlymapping isallowed forfreelist creation. Flagguards+fulladapterASan/UBSan tests,fullhostlibrarybuild andliveGPU-onlyprotectedfreelistlifecyclePASS. NoTAjob/ARM64ICDclaim. NormalUSCgen was regenerated afterlast-burst sourcefix, so earlierstaleartifact note is resolved.
