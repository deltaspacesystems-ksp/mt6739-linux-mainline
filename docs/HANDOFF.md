> AKTUALIZACJA 2026-10-28: stock replay renderuje wg testów Claude; aktywny GPU kexec pozostawił domenyON i nowy KM odmawia init. Oczekujemy pełnego power cycle od właściciela. Wyniki bisekcji, pomiar SPM i hybryda offline: [GPU_PHASE2_HANDOVER.md](GPU_PHASE2_HANDOVER.md). Wcześniejsze wpisy poniżej są historyczne.

> Pakiet renderu64x64: [GPU_RENDER64_PACKAGE.md](GPU_RENDER64_PACKAGE.md) — sklad, adresy, PASS/nieprzetestowane, odtwarzanie, staging i pozostaly submission. Manifest8plikow: GPU_RENDER64_MANIFEST.json. Pierwszego draw nadal nie wykonano.

> BLOCK externalstate:USB/RNDIS disappeared duringpayloadtransfer (Win10054,ping/telnettimeout,adapterabsent). NOdraw/reboot/flash/kexec. Owner screenphoto needed. graphics64 bundle8files ready;only3transferACK, pds-context unknown,stageclient notuploaded/executed. RecoverUSB/checkuptime/GPU/logs/listener thenSHAall8/nativeStage beforedraw. gpu-graphics64-bundle-staging.json.

> LIVE fullgraphics memory footprint32PMR/5heaps PASS,30CPUchecks+2protectedtables,cleanupRC0/HWR1unchanged. OfflineTA112contextpack+stockoraclePASS,VDMsyncnativeNOP8B0temps. NoTA/3Djob. OldKICKbridge unconditionallyrequiresvalidPRFenceSyncPrimBlock. Nextassembleimages/FWpackets/PRsyncsemantics. gpu-graphics-memory32-preflight.json/gpu-ta-context-sync-candidates.json.

> CORRECTION finalVSmetadata=5outputs (implicitPSIZE),not4; VDMfixed5,PPPoutput05040000. FSdiscard1=>PUNCH_THROUGH,phase0. PPP25/contextPDS candidatesPASS. StockbuffersVDM92/TA484/PT128/SR4030hex,2banksDWARFconfirmed. NoTAjob. Target expanded acceleratedWayland/WebGL2; automationPAUSED byowner. gpu-graphics-final-contracts.json.

> Stock-selectedCAM31/maxInstances16 recovered+ARM64checked for4output/raster. OfflineVDM8word incomplete segment/PBE RGBA8linear64/EOT40B2temps+PDSevent allcandidatePASS. NoGPUgraphicsjob; PPP/ISP/contextswitch/fences stillneeded. gpu-vdm-pbe-eot-candidates.json.

> KICKTA3D exactbridge130/14 input352/output16 storage+alloffsets host/nativeARM64PASS, independently stockwrapper e26c agrees; stockCMDTA280/CMD3D664 agreesDWARF. No submitAPI/nojob. gpu-render-bridge-layout-validation.json.

> ExactrenderFW storageheader host+nativeARM64 staticassertPASS:TA104/3D608/CMDTA280/CMD3D664; unknownZSlice opaque. No submitAPI/nojob. Next full VDM/PPP/ISP/PBE/EOT assembly. gpu-render-wire-layout-validation.json.

> Graphics PDS offline candidatesPASS:VS vertexID->vi0;FS kick9temps;FSmeta202->shared0 (aligned16). StockHWRT geometryfield mapping recovered, PPPgrid0 forflags0 (priorgrid_candidate isdifferentfield),TPC+VHeap layout128+1536 for64x64. No triangle/hardwaregraphics proof. See gpu-graphics-pds-candidates.json/gpu-stock-hwrt-field-map.json.

> Exact stock v1 geometry independently ARM64-correlated:tiles16x16,region align64(5bytes/tile),tail pow2square*8 (single sample/layer). 64x64=>region128/tail128. Mesa mtile init assertsv2 and cannot be used unchanged for targetv1. Bridge field mapping pending; noTAjob. gpu-v1-render-layout-correlation.json.

> Offline bounded VS/FS candidates compile without physical depth/slots:softwareRA20,VS176B/6temps,FS440B/9temps/1shared fs.meta. Budget8 FS spill guard correctly stops. No upload/renderproof. Next PDS FSmeta0x202 delivery + real render backing/VDM/ISP/PBE contracts. gpu-graphics-bounded-candidates.json.

> HWRT LIVE CREATE/DESTROY PASS exact130/0,1 input98/output32; LOCAL0/GLOBAL1/maxRTs1, complete12PMR cleanupRC0/HWR1 unchanged. Lifecycle ONLY: geometry zero,4K placeholder owned backing not render-sized. Next derive real render backing/geometry and VS/FS/streams. No triangle. gpu-hwrt-lifecycle.json.

> RenderTarget LIVE CREATE/DESTROY PASS, owned zeroed4K address backing (lifecycle only), cleanup12PMR RC0/HWR1 unchanged/CRR0. No TA/3D. Next HWRT and actual backing sizing. gpu-render-target-lifecycle.json.

> Protected global/local freelist pair LIVE PASS:32pages each, cleanup12PMR RC0, HWR1 unchanged/CRR0/FW OK. Exact HWRT freelist order LOCAL0/GLOBAL1 (count2). Next RenderTarget/HWRT lifecycle; still NO triangle. gpu-freelist-pair-validation.json.

> PMprotected freelistPASS:modernbit26->oldbit24 translation guardedGPU-only,allotherdevicehintsreject. LiveCREATE/DESTROYcleanupRC0/noHWRincrement; flag+fulladapterASan/UBSanPASS/fullhostMesa rebuildPASS. NormalUSCgen refreshed (stale note resolved). Newpatchmesa-ddk19-pm-protect.patch. Still noTA/3Dpixel.

> LIVE freelist32pages130/8,9 CREATE/DESTROYPASS withGPU-onlytable,cleanup11pagesRC0,FWOK/HWR1unchanged. Table currentlyNOTPMprotected; beforeTA explicitly translate modernPM_FW_PROTECTbit26->legacyPMMETA_PROTECTbit24 (adapter currentlyrejects). Lifecycle is not renderproof. gpu-freelist-lifecycle.json.

> LIVE rendercontext130/12,13 CREATE/DESTROYPASS withowned4KVDMstack,disabledframework,cleanupRC0; noHWRincrement. ExactFWrenderDWARF CMDTA280/CMD3D664/HWRT328/Freelist64 recovered. Nextfreelist table MUSTbeGPU-only (kernelrejectsCPUread/writePMR). No triangle yet. gpu-render-context-lifecycle.json.

> SECOND independent nativecomputePASS:newcompiledliteralA1B2C3D4/newVA65c0000040(word16),UFO/1023guards/cleanupRC0. HWRbaseline1unchanged,CRR0,idle0%. Actualcomputeproven, NOT triangle/fullMesa. gpu-native-usc-challenge-pass.json. Owner mistaken triangle claim corrected. NextTA/3Dcontext/freelist/HWRT andVS/FS.

> BREAKTHROUGH LIVE nativePCO USC STORE PASS:outputdeadbeef->12345678,UFO12345678->87654321,1023guardwordsPASS,complete12page/5heapcleanupRC0. FixMMU_INIA/INIB BRN65273 mappings. FW/DriverOK,HWRbaseline1unchanged/CRR0,idle0%. No reboot/Bionic. Not3D/fullMesa/profile. gpu-native-usc-store-pass.json. Nextchangedliteral+address thenTA/3D.

> FIRST native USC store KICKaccepted butFAIL:UFOdone/outputunchanged,HWR0->1,FW/DriverOK,cleanupRC1. Recovery fault writesF800000000 (TAregionheader), matching missingBRN65273MMU_INIA. Stockcontextcreator allocates5B/align512+mapsbothINIA/INIB; addowned4Kcoverage andstaging before ONEnewtest. BaselineHWR1, no blindretry. gpu-first-store-trial-failure.json.

> LIVE compute memory footprintPASS:10PMR4K pages mapped across queriedGeneral/PDS/USC, CPUzero/pattern readback and complete reversecleanupRC0; FWOK,HWR0/CRR0. ActualBRN65273 heaps confirmed. No shaderjob/reboot. gpu-compute-memory-preflight.json; native source archived.

> Native open contextUSC store/load candidates now compile with explicit softwarebudget20+unknowncapacity,10temps each,568/488B. Fixed public common.cl finalpartialDMA unsignedunderflow;1539CPUmodelcases+SPIRVbuildPASS. NoGPUupload/ISAclaim. Apply newmesa-context-sr-last-burst.patch; normalgenerateduscgen currentlystale. gpu-native-context-usc-candidates.json.

> Restricted standalone RA softwarebudget8 compiles WG1 store WITHOUT unifieddepth/slots profile values (depth0):128B,4temps/1vtxin/no spills. Different register assignment vs synthetic64/4. NOT hardware limit/ISA proof; production compiler untouched. gpu-pco-budget8-experiment.json. No upload.

> CORRECTION+PASS: stockcontext PDS DOES emit2DOUTW via helper36230 beforeDOUTU; previous decompiler omittedargs. Complete public PDS pair matches16data/6code words for store/load against oldexpressions; harnessASan/UBSanPASS. Earlier different-pointer-contract claim retracted. USCISA/profile still unresolved. gpu-pds-context-pair-comparison.json.

> BREAKTHROUGH exact loaded GPU firmware includes fullDWARF/symbols! Types independently confirmruntime56/context56/command120,flags112/deadline116; runtime names nowEXACT, offset48TPU_ARRAYS_CDM. microMIPS consumer/register correlation saved. Blob/decomp PRIVATEonly; no shaderkick. gpu-exact-firmware-compute-dwarf.json; extractor archived.

> Found independent public historical computeFW header with matching120B shape:runtime56/context56/flags112/deadline116. Candidate register names recovered; host+ARM64 layoutPASS. Historical3.18 source NOT proven exactDDK1.9; optional features and unwritten48..55 remain unresolved. gpu-public-legacy-compute-layout.json. No submission.

> Morning: public computePDS generator candidatePASS (localX->vi0,kickUSC):3dataDWORDS/20codeBYTES=5instructions. Return-pointer/canary/ASan-UBSan harnessPASS; SLCfeature0/1 outputidentical. Actual targetCDMinput/ISA/oldpacket/context still unresolved. No phone upload. gpu-compute-pds-input-candidate.json.

> Final grouped computeIR audit:12groups,5temps/1vtxin;vi0 controls local-index<1 conditional execution. PDS must supplyvi0=0, not undefined. Storewritethrough+DRC0/WDF; targetISA/errata/cache and oldpacket/context still unverified. gpu-restricted-compute-ir-audit.json. No upload.

> Restricted compute post-lowering NIR whitelistPASS:1 invocation-index load+1global store, no tex/other intrinsics; synthetic64/4 offline128B/5temps. Narrows interpolation/texture path review, NOT hardwareISA/profile validation. gpu-pco-query-sites.json; probe guard archived. No upload.

> CORRECTION offline sweep:depth16 underflows unsigned reserved-line subtraction; historical25/30success count includes invalid cases. Marked16/32INVALID, added probe rejectionPASS. Synthetic64/4 compute metadata5temps/1vtxin/0shared,128B. NOT hardware limits/dispatch. pco-normal-shader-sensitivity.json updated.

> StockGLES compute caller initialization indexed:112B input zeroed, runtime constants/resource provenance saved. This DOES NOT initialize unresolved FWpacket48..55 in separate wrapper. Register identities still unknown; common-size input is per-task, not capacity. gpu-compute-caller-map.json. No phone changes.

> LIVE Y5 native compute-context CREATE/DESTROY PASS exact1.9 bridge129/0,1;16B disabled framework, no KICK/shader. Native compile/cleanupRC0,FirmwareOK,HWR0/CRR0, no reboot. gpu-compute-context-lifecycle.json; archive compute19_context_trial.c. Not3D; profile/programs/packet still unresolved.

> ActualHuawei clock globals now matched to exact kernel strings:VDEC power SMI1->MT_SCP_SYS_VDE->MT_CG_VDEC, off reverse. This closes name-correlation gap, not native provider/SMI ownership. Evidence updated vcodec-huawei-stock-correlation.json. Engines untouched.

> ActualHuawei4.4 VDEC decomp confirms IRQack and SMIid1/two-clock sequencing from reference. Symbol index shows only weakscp_dump_pc/lr, no VPUcodec transport symbols (absence not silicon proof). Clock globals still need probe correlation. vcodec-huawei-stock-correlation.json. No engine touched.

> VDEC IRQ ack protocol matches Linux7.3 (MISC+a4,OR11/AND~10), narrow reference evidence only. MainlineH264 backend requires VPU protocol not supplied by stockioctl; no blind compatible addition. SMI before clocks and failure unwind required. vcodec-irq-power-comparison.json; engines untouched.

> Codec fallback audit:7 reference source hashesPASS; stock ioctl is control/MMIO/memory plumbing, HAL command cases are stubs. Native frame parsing/register programming still missing; ioctl port alone cannot decode. HEVC_ENC enum not hardware proof. No engine touched. vcodec-native-interface-audit.json.

> USC capacity provenance correction:state+1150 in regallocator points to caller program params (CompileToHw state[22a]=param4), NOT discovered hardware table. New init/range-helper audit yields no physical limits. Actual unifieddepth/slots remain UNKNOWN. gpu-usc-capacity-provenance.json. No phone changes.

> BRN65273 source:USC/PDS heaps each1GiB, so32bit offset guard insufficient; added full-span queried-heap bounds helper, sanitizer/ARM64PASS. BRN63553 is MIPS firmware boot-remap workaround, not USC or kexec proof. gpu-errata-heap-startup-audit.json. No phone changes.

> USC shared-DMA instruction path indexed; legacy errata source index saved. 65273 affects USC/PDS heap layout;63553 has startstop guard. PinnedMesa lacks shader workaround evidence for other target errata; absence is not safety proof. gpu-context-instruction-audit.json. No dispatch.

> Found native open USC context store/load sources and built artifacts544/488B (temps10/10), plus16dw variant1040B. Generic COMMON compiler profile64/14 NOT target evidence. Public PDS DOUTW state-pointer contract differs from stockminimalDOUTU; do not mix programs. gpu-public-context-programs.json and audit script. No dispatch.

> USC/PDS address origin resolved by static allocator call chain: subtract selected heap base when relative flag1; compute-state General keeps fullVA(flag0). Guarded native heap_offset helper +sanitizer boundaries/ARM64compile PASS. No USC program/dispatch yet. gpu-pds-fence-comparison.json.

> XML TEMPS consumer audit:384 public generated pack/unpack cases PASS, no rescaling; cmd_buffer only changes sample_rate. Unused UNIT_SIZE2 macro does not imply /2 translation. Preserve encoded units2/5. Execution-address bit expression agrees, heap origin still unresolved. No phone changes.

> USC context task-control bases decoded offline:store TEMPSunits2/load5, instance rate, dualphaseOFF; public-mask assertions PASS. Per-task units NOT hardware capacity. setup_doutu /4 convention vs XML UNIT_SIZE2 needs consumer audit; execution-address heap semantics unresolved. No GPU dispatch.

> PDS generic USC kick encoder now matches stock expression for256 offset/predicate pairs offline;8dword data/1code layout agrees. Entire modern context store/load generators NOT validated (extra DOUTW/feature barriers). USC task control/profile unresolved. No phone changes. gpu-pds-fence-comparison.json.

> Offline public Mesa PDS encoders reproduce all5 stock fence/terminate instruction words PASS. Component ISA evidence only; software-barrier feature requirement/USC/profile/full packet unresolved. No phone changes. gpu-pds-fence-comparison.json; archive compare_pds_fence.py.

> CDM store/load/fence PDS1 controls now source/public-mask matched and guarded nativeC;16192unified/data pairs perSTORE/LOAD+boundaries ASan/UBSanPASS, ARM64compilePASS. Seven oldcontext words mapped byrole to modernstatic/per-kick state; alternateB-state not mapped. No full packet or program bytes/dispatch yet. See gpu-compute-state-packing.json/latestNOTES.

> CDM common-size field helper independently ARM64/public-mask confirmed: round(dwords*4,64), shift11/maskff800. Native guarded pack header +8177-value ASan/UBSan/public staticassert/ARM64 compilePASS, no dispatch. Stock shared-state allocation0x4030B is NOT hardware capacity. PDS store/load/fence helper roles partly identified; next actual program contents and runtime packet fields. gpu-compute-state-packing.json.

> Stock CDM120B byte-map recovered/ARM64checked: runtime0..47, unresolved unwritten48..55, context56..111, flags112/deadline116. Host offsetof PASS. CREATE allocates72B/alignment16 compute-state; PDS pointer packing matches public masks, exact roles unresolved. No packet builder/dispatch/phone changes. Next private helpers2f650/2fa30/2ff10/30044, see gpu-compute-packet-map.json.

> OWNER UI DIRECTION: macOS 27 is the current style target, superseding the earlier general iPhone/iOS-copy goal. Preserve native C/C++ desktop/windows, English, compact scale, gestures/multitouch. Previous photos remain references for individual glass/motion effects where consistent. See UI_STYLE_REFERENCE.md/latest NOTES.

> Compute ABI audit: oldCREATE32/KICK132 vsMesa56/108, oldkick2 vsmodern5. Stock FW payload120B independently confirmed ARM640x20fdc. Exact public1.9tree lacks client FW packet header; do not use modern FW body unchecked. Old caller embeds7context-derivedqwords per kick vsmodern staticstate at creation. Next reconstruct120-byte packet/register semantics and retained context state. Source-only, no phone changes. gpu-compute-submission-audit.json.

> NIGHT continuation configured heartbeat microfw-nocna-praca-gpu (20min,24runs) ACTIVE. FirmwareNULL executionPASS. Offline normal PCO shaders compile with exact metadata allocators and SYNTHETIC capacity inputs:25/30cases each, unique binaries compute128B/VS176B/FS440B. NOT hardware validated/profile still incomplete; do not upload synthetic-profile code. StaticGLES per-program budget0xf8 is NOT unified-store depth. Updated plan next.

> FIRST firmware-queue execution PASS: exact1.9 RGXKickSync NULL + UFO update, CPU observes12345678->87654321, cleanup/FWOK/HWR0/CRR0. Not shader/3D. PCO offline store blocked by unknown unified_store_depth;14/88profile fields reference-known,74unknown. Private staticlibsrv reference verified/unmounted, no Android execution. See gpu-firmware-null-command.json and gap audit.

> GPU transfer-context CREATE/DESTROY PASS on phone through restricted Mesa adapter;18operations. Disabled reset-framework16B verified, no task/register payload submitted. Driver/FirmwareOK,HWR0/CRR0, no reboot. CRITICAL modern submit3 is legacy setpriority3; legacy submit2/layout differs. Translate explicitly. README + mesa-transfer-context-validation.json.

> UI motion built natively on Y5 and launched without reboot, 600s event-driven desktop + Settings; frame callbacks720x1440 PASS. Rollback binary pre-motion. Touch NOT started: legacy power trial unbinds PMIC now providing GPU VCORE; needs dependency-safe path. No physical animation/FPS validation yet.

> UI motion C added: cached420ms critically damped page slide/parallax/dim/rounded corners, interrupt-continuous, frame-paced in xdg client+desktop base layer. ASan/UBSan motion and Weston/labwc regressionsPASS. PC only, no phone FPS/deployment. Foreign-window compositor transforms, finger-driven progress/rollback, blur/refraction pending.

> Owner provided9 UI style references: dark grouped cards, glass chrome, interactive sliding/parallax and refractive gesture edges. docs/UI_STYLE_REFERENCE.md records requirements/sequence. Initial C renderer Settings styling+glass highlights built/previewed onPC only; no phone deployment/animation/blur/refraction yet.

> Mesa current-process PMR PID now accepted, matching actual getpid caller; foreign PID rejected. Phone PMR/map+syncreadback regressionPASS, host sanitizers/build/replayPASS; no restart/render. Heap carveout_size really consumed by heap-init and remains unresolved, no invented zero. Apply current-pid patch after stage4.

> Sync memory readback PASS: explicit legacy make/import -> read-only mmap sees12345678 set by SYNC2 -> unmap/unref7/unmake4/free parent. Exact helper ASan/UBSan seven failure pathsPASS. No GPU completion/render/restart; modern adapter still16 ops. FirmwareOK/HWR0/CRR0. See mesa-sync-readback-validation.json.

> Mesa stage4a sync block ALLOC/SET/FREE PASS on phone, 16 operations; no GPU completion/render yet. PMR regressionPASS, FirmwareOK/HWR0/CRR0, no restart. Export handle is PMR_LOCAL_EXPORT_HANDLE child, not ordinary PMR. Heap reserved_size and nonzero pid compatibility remain blocked. README and mesa-stage4-validation.json.

> Mesa stage3b PMR allocate/free + GPU map/unmap PASS on phone through exact adapter (4KiB CPU zero/pattern check, cleanup), 13 operations. ASan/UBSan+host library+patch replay PASS. FirmwareOK/HWR0/CRR0, no restart/render job. Restricted single-chunk4K,pid0,verified cache flags only; out_flags unavailable. Next heap/sync/submission and target profile.

> GPU resumed by owner: Mesa stage3a reservation/unreservation adapter PASS on phone (4096 bytes, full cleanup), nine supported operations. Host library+ASan/UBSan+patch replay PASS. FirmwareOK/HWR0/CRR0, no restart, no rendering. Next PMR/cache flags/map, then sync/jobs; compiler profile unresolved. phone/mesa/README.md.

> MC3xxx ID measured 66/23 at I2C1/0x4c; stock4.4 validation accepts pair. Identity only, no accel config/calibration writes. Restricted module persisted; optical IIO restored, IR=N. GPU postponed; radios require shared PWRAP/VCN + CONNSYS/BTIF. See latest NOTES and phone/hardware/mc3xxx-identity-v62.json.

> USER PRIORITY CHANGE: GPU postponed. Work modem/WiFi/BT/CONNSYS and all actual sensors now. Preserve Mesa stage2. Opticalphysical testsPASS; nextMC3xxx identification, radioPMIC/clock/EMI dependencies.

> PHYSICAL OPTICAL PASS v62: continuous ALS cover0/lamp29897; IR far14-23 vs hand182-268, both kernel-trialrestorePASS. IRgate nowN/IIOlightback. Per-readIIO500ms changed, freshness retestpending. MesaABIstage2(nativecontext/heap) phonePASS; no render. Evidence optical-physical-tests-v62.json and mesa-stage2-validation.json. No restart.

> MESA STAGE1: full host PowerVR Vulkan library builds after LLVM23/NIR fixes; experimental1.9 CONNECT/DISCONNECT adapter tested, untranslated commands reject. NOT phone-rendering-ready. Exact7file patch replayPASS. phone/mesa/README.md. StaticUSC tile-buffer constant12KiB, no completeGE8100profile; privatebinary notinrepo. Phonev62/GPUFWhealthy, no restart.

> EXACT4.4 WMT/CONNSYS analysis: EMI remap10001380 live0(disabled); formula/reset verified in ARM64 instructions. PMIC enum fields decoded to real MT6357 registers; no writes/power/radio/NVRAM activation. Stock STP uses BTIF. Source phone/ghidra/wmt_consys.c,k44_pmic_fields.c; evidence phone/hardware/consys-stock-v62.json. Owner away, continue source/native modules.

> GPU VA PASS on v62: native context/heap/reserve/map/unmap/destroy of4KiB at65c0000000; FW/DriverOK,HWR/CRR0. No render job. New docs/NATIVE_RENDER_MEDIA.md maps zero-copy and H264/HEVC/audio/camera prerequisites; owner away, no new restart. phone/hardware/gpu-va-v62.json.

> V62 new hardware: native I2C1 diagnostic+stock-c2 LIGHT IIO modules loaded without reboot; IDc2 measured, rawALS16/channel0a54-56, CTRL0 restored after each ALS-only sample. No PS/IR. Exact chip/lux/physical response unverified; owner away. GPU FW+PMR CPU map PASS after kexec, no rendering yet. phone/kernel/sensors and phone/hardware/light-iio-v62.json.

> KEXEC PASS v62/#48 -> same v62/#48: definitive /chosen/linux,booted-from-kexec marker, uptime reset, USB/shell/4CPUs/Charging returned. No boot write in trial. GPU N. Different-kernel and active GPU handover not yet tested; module autoload not configured. Evidence phone/hardware/kexec-v62-trial.json; archived bounded trial scripts.

> OWNER STANDING AUTHORIZATION: subsequent MICROFW kernel/boot/rootfs updates and restart/kexec tests may proceed without per-version approval. Keep backups/readback and boot/rootfs-only boundaries; later no-restart instructions override. Kexec handover NOT yet tested, automation not proven. See latest phone/NOTES.md and docs/BUILD_AND_FLASH.md.

> V62 RUNNING #48 confirmed: USB/shell/Charging PASS. Kexec API present loaded0/load_disabled0; no jump tested. Installed41 modules; IIO/BT/GNSS/WWAN/ASoC framework modprobe PASS, Not tainted. GPU default disabled. Physical devices not yet operational. No second restart. phone/hardware/mfw-v62-validation.json.

> V62 BOOT WRITTEN: owner approval, full image readback SHA256 PASS, reboot requested. Await owner shortPower on LK and #48 startup verification. Modules remain on PC, no kexec jump attempted. phone/hardware/mfw-v62-validation.json.

> V62 READY #48: kexec_file+CPU hotplug+loadable native hardware frameworks, 41modules/depmod/bootvalidator/backup/RAMstaging PASS. NO boot write/restart; phone v61. Signed Alpine kexec-tools installed (version/help only). Hardware operation and kexec handover NOT established. Await specific boot-write approval rule9. docs/MFW_V62_KEXEC_HARDWARE.md.

> V61#46 RUNNING: ownerPower/startPASS, nativeGPUbind/card1/renderD128, nativeC CONNECT+17heapqueriesPASS. DebugfsDriverOK/FirmwareOK,HWR0,CRR0,APM0; GPU IRQ17/SPI226count1,VCORE1.15V,Charging. NOshader/rendering/zerocopyyet. OWNERAWAY: NOFURTHERRESTARTS; continueGPUuserspace, ormodem/WiFi/BT/hardwareifblocked. EntireCPUstacknative, noBionic/HAL. Latestphone/hardware/gpu-native-v61-validation.json.

> V61 BOOT WRITTEN after explicit approval: complete image SHA256 readbackPASS, rebootrequested. AwaitownerPower/start; no v61startup orFWresultyet. Target#46. Previousphonev60boundbutDRMopenblocked; v61fixesmandatoryfopsflag.

> V61 PREPARED: phonev60#45 write/readback/start/bindPASS, card1/renderD128. FirstOPEN blocked BEFORE firmware bymissingDRM7.3FOP_UNSIGNED_OFFSET. V61#46 soleflagfix built+validated, DT19/config/initramfsunchanged; deltaONv60 source. Backup+RAMstagingPASS, NOv61write yet. Awaitownerconsent(rule9). NativeCPUstackonly; noFW/renderPASS yet. NativeheapclientmocktestsPASS. GE8100marketing(primaryMediaTek), exactHW22.49.21.16; do notborrowGE8300profile.

> V60 PREPARED 2026-10-26: native GPU kernel#45, image/DT/config/boot validator PASS; full627-file patch pristine dry-runPASS; private backup24MiB+RAMstagingPASS. NO BOOT WRITE/reboot, phonev59. DefaultGPUbindingoff; oldIDprobedisabledinDT19. First render OPEN loadsFW; inspectbind beforeexplicitnativeCconnect. NativeMesa1.9/GPU22 port and rendering remainpending. Awaitownerbootwrite/restartapproval(rule9). docs/BUILD_AND_FLASH.md latestsection.

> NATIVE GPU UPDATE: owner requires NO Bionic/HAL runtime. Full DDK1.9 ARM64 archive/link and all symbol resolution vs v59 vmlinux PASS; exact replay+sanitizers PASS. Native C ABI probe runs on phone Alpine in /tmp, noGPU touched. Open Mesa pvrsrvkm backend exists but needs real1.17->1.9 bridge/GPU22 port. New power/platform default disabled, not inImage yet. Phone staysv59. Continue kernel integration/controlledfirmwaretest. docs/GPU_DDK19_PORT.md and phone/hardware/gpu-native-port-build.json.

> GPU PORT UPDATE: seven fresh ARM64 DRM/dma-buf/native dma_fence objects W=1/-Werror PASS; exact-source replay and ASan/UBSan fill-failure tests PASS. Cumulative ten-file overlay in phone/kernel/gpu/ddk19. Still no linked driver or acceleration, phone unchanged v59. Next OS/platform power/ION/linkage. See docs/GPU_DDK19_PORT.md.

> GPU DDK1.9 PORT: public exact-version sources verified (590 blobs); initial DRM object ARM64 W=1/-Werror and clean replay PASS. Compile-only, no linked driver or acceleration. Memory/fences/OS APIs still blocked. Phone remains v59/#44; no boot write/reboot. See docs/GPU_DDK19_PORT.md.

> BIEZACY v59#44 POTWIERDZONY: zgoda/bootwrite/readback/reboot/Power/start PASS. PierwszyrzeczywistyHWIDGPU: CORE_ID08470000/PBVNC0016003100150010 ->22.49.21.16, przy26MHz/VCORE1.15V. Testerresult0/faulted0, poprawnyOFFobuMFG iTOPgategaszony; VCORE iPLLbez zmian,Charging28.7C,displayunderflow0/abnormal0. GPUterazOFF; BEZakceleracji/firmware/rendernode. Dowody phone/hardware/gpu-hwid-v59.json i koniec docs/DRM_GPU_BRINGUP.md. Aktualnyobrazmfw_v59.img, awaryjnyv58.

> NOWE: mfw_v59.img #44 GOTOWY (CCF26MHz+readonlyVCORE+boundedGPU_IDprobe), build/W=1/UBSan/bootDTvalidator/full31-filepatchdry-run/backup+RAMstagingPASS. JESZCZE BEZ zapisu/reboot, telefonv58#43/GPUOFF. Bootv59 nie wlaczaGPU; explicitread-id sprawdzaVCORE>=1.05V,26MHz,SPM/SRAM/busack, dopieroczytaCORE_ID/PBVNC i wylaczaGPU. To testerID, BEZakceleracji/firmware/HWAPM. Potrzebna zgoda na konkretny zapis/test. Source/procedura phone/kernel/gpu orazkoniec docs/DRM_GPU_BRINGUP.md.

> BIEZACA PRACA GPU: telefon nadalv58#43, GPUOFF. CCF26MHz probeclock z rejestrami zweryfikowanymi w4.4/publicznymMT6739 przygotowany i ARM64W=1/UBSanPASS, JESZCZE NIE wImage/DT. Publiczne referencje przypiete do commit47f925eac07c660002cb85939c94cd8a191e1c53 (bezremote). Kolejna zaleznosc: rzeczywisty odczytVCORE i kontrolowanypower/HWIDprobe; regulator/MFD/PMframework brak. Nadal bezakceleracji. Szczegoly koniec docs/DRM_GPU_BRINGUP.md i phone/NOTES.md.

> BIEZACY v58#43 POTWIERDZONY: zgoda/bootwrite/readback/reboot/Power/start PASS. Fizycznytest nowych DMA0x44800000->0x44c00000->oryginal0x775eb000 PASS, wlasciciel zobaczyl oba kolory i poprawny powrot. Driverresult0/restore0/underflow0/abnormal0, bez DSI kick. Dwa buforystestowe pozostaja pinned do reboot. Nadal BEZ KMS/GEM/pageflip i GPUakceleracji; to potwierdzona podstawa nowegoDRM. LadowanieCharging30.3C. Aktualny obraz mfw_v58.img, awaryjnyv57. Dowody phone/hardware/display-dma-trial-v58.json i koniec docs/DRM_GPU_BRINGUP.md.

> NOWE: v58#43 DMA scanout test GOTOWY (kernel/boot/DTvalidator/full24-filepatchdry-run/backup+RAMstagingPASS), JESZCZE BEZ zapisu/restartu, telefonv57. Test wymaga wlasciciela przy panelu: green5s,magenta5s,oryginal; tylko jawne sysfsrun, bez zmianDSI/zasilania. OVLport0IOMMUbypass potwierdzony kodem4.4+liveLARB. GPU: stock rgx.fw.signed znaleziony readonly na vendor, as-is odrzucony przez naglowekLinux7.3; stock ma zaleznosc wspolnegoVCORE/DVFS. GPU nadalOFF, bez akceleracji/KMS. Szczegoly koniec docs/DRM_GPU_BRINGUP.md. Test600sUI/touch zakonczonyEXIT0,gestywlascicielpotwierdzil.

> POTWIERDZENIE WLASCICIELA: po async telemetry na v57 UI jest wyraznie lepsze, gesty dzialaja. Fizyczny test pozytywny; nie uznawac tego za weryfikacje GPU/zero-copy ani usuniecia wszystkich artefaktow. RDMA underflow0/abnormal0, Charging30.3C. Test600s jeszcze RUNNING przy odczycie; szczegoly na koncu phone/NOTES.md.

> BIEZACY: v57 #42 POTWIERDZONY po zgodzie, boot/readback/reboot/Power/start PASS. IRQ14 MT_SYSIRQ202 dziala: delta300EOF/5s=60Hz, underflow0/abnormal0. LadowanieCharging/temp30.3C. UI: zmierzono blokade telemetrii50-56ms/s; wdrozono osobny worker C/pthread, snapshot avg347ns, testy worker/Wayland PASS. Aktualny600s test touch+UI-async uruchomiony, fizyczna ocena gestow jeszcze oczekuje. BackupUI pre-async. GPU OFF; IRQ observer nie jest KMS/zero-copy. Szczegoly na koncu docs/DRM_GPU_BRINGUP.md i phone/NOTES.md.

> NOWE: mfw_v57.img #42 (IRQ observer RDMA, bez modeset/GPU) przygotowany, image/DT validator i full23-file patch dry-run PASS, prywatny backup+RAM staging PASS. JESZCZE BEZ zapisu/restartu; nadal v56. Potrzebna zgoda wlasciciela na konkretny zapis boot. Szczegoly na koncu docs/DRM_GPU_BRINGUP.md. Nie mylic obserwatora IRQ z KMS/zero-copy.

> BIEZACY STAN 2026-10-25: v56 #41 POTWIERDZONY (boot/readback/start PASS), DSI video1/hz0 bez manualnych kickow. Cache tla C wdrozony i szybszy: desktop render110ms->13ms; 72 porownania RGB i test-desktop PASS. Nadal sporadyczne problemy obrazu, dotyku i okien/gestow; animacji brak. Ostatnie600s testy UI/touch zakonczone EXIT0. RDMA daje okolo60 EOF/s, dobry punkt wyjscia dla DRM pageflip, jeszcze bez aktywnego KMS/zero-copy/GPU. GPU OFF. Wlasciciel jest przy telefonie, restarty dopuszczone. Szczegoly na koncu docs/DRM_GPU_BRINGUP.md i phone/NOTES.md; ponizsze statusy sa historyczne.

> BIEŻĄCY: v55 start/readback PASS (#40, poprawionyDTv15), DT i rzeczywisty OVL3 wskazują775eb000/pitch2880. Fizyczny dotyk MStar wWaylandzie potwierdzony wcześniej, mosttestowy zakończony/posprzątany. Obraz w ruchu nadal wymaga testu. NOWE: OVL3->RDMAdirect-link->DSI VIDEO(sync-pulse1), nie command! MagentaRAM-only bez triggera zobaczona przez właściciela. Rzeczywisty MUTEX14001000; stary bootstrap pisał14015000 i restartował ciągłystrumień. V56 #41 pomijający ręczne kicki dla video GOTOWY, build/bootvalidator/pełnypatch22pliki dry-run/stagingPASS; pytanie o zapis/restart oczekuje. GPU domenyOFF (SPM bit4 i31), rdzeńnieodczytywany. Pierwszy komponent walidacji OVL/GEMDMA ma UBSanPASS, nie jest jeszcze sterownikiemKMS ani akceleracją. Szczegóły docs/DRM_GPU_BRINGUP.md.

> AKTUALNE 2026-10-25: v54 #40 POTWIERDZONY, zapis boot/readback/start PASS, uinput działa. Z kodu4.4 odtworzono MStar: ACK62/komenda37, normalne ramki26/43B/5a/checksum i realny kontakt. Właściciel potwierdził dotyk w Waylandzie z poprawnym trafianiem. Test180s zakończony/sprzątnięty; ładowanie działa. OBRAZ nadal migocze/rozrywa się podczas interakcji i zostają stare fragmenty; brak flickeru był tylko na spokojnym desktopie. Testowy czytnik ma wiele badframes i nie jest autostartem. Najnowszy priorytet: właściwy DRM/RDMA/DSI/zero-copy i GPU. Pełne dowody/ograniczenia: docs/MFW_V54_TOUCH_BRINGUP.md.

> AKTUALNE: właściciel wrócił do telefonu i ponownie pozwala na testy przy nim. Desktop C/Wayland widoczny BEZ MIGOTANIA — odpowiedź właściciela 2026-10-25 podczas nowego testu event-driven hz0 z ograniczonym odrysowaniem UI. Telefon nadal v53/#39, uptime14h37 przy rozpoczęciu. V54 z uinput/I2C_CHARDEV zweryfikowany, przygotowanie backupu/staging; jeszcze BEZ zapisu boot/restartu. Kolejny krok: dotyk. Nie mylić przygotowania obrazu z działającym sterownikiem dotyku.

> NAJNOWSZY ZAKRES: kamery, ALS/proximity, mikrofony i GPS. Bez restartów podczas nieobecności właściciela. Kernel v53 nie ma V4L2/ALSA/IIO/adapterów I2C ani modułów; zdjęcia/nagrania jeszcze niemożliwe. Odtworzono14 wariantów sekwencji kamer (nie tożsamość matryc), LTR577 I2C1/0x53 i zależności kodeka MT6357. Native C diagnostyka LTR577 skompilowana, zatrzymuje się na braku adaptera; testy przechwytywania przygotowane bez autostartu. docs/CAMERAS_SENSORS_AUDIO_GPS.md zawiera dowody, błędne tropy i plan.

> STAN BIEŻĄCY 2026-10-25: właściciel jest poza domem. BEZ restartu, poweroff, suspend i aktywowania nowego kernela; test panelu dopiero po jego powrocie. Nadal v53 (#39). Desktop C/Wayland ZOBACZONY przez właściciela, ale z dużym migotaniem przy 25 Hz. Ustawiono y5disp/hz=0; poprawa wizualna NIEPOTWIERDZONA. Próby GUI zatrzymane. V54 z uinput/I2C_CHARDEV GOTOWY, NIE wgrany. Native build optymalizacji bezczynnego UI PASS, jeszcze bez próby panelu. Bieżące dowody: docs/UI_TOUCH_RADIOS_20261025.md; plan radia/zegarów: docs/RADIOS_CLOCKS_BRINGUP.md.

> BIEŻĄCA PRACA: docs/UI_TOUCH_RADIOS_20261025.md. NativeAlpine/aarch64 UI zbudowane na telefonie, /dev/shm naprawione. Trwa debug widoczności panelu mimo poprawnych frame callbacks i buforaDRM. Dotyk GPIO119/6,VLDO28off2.8V. WSL buduje54(UINPUT/I2CCHARDEV), jeszcze bez flash; Ghidratouch/radio w tle. Właściciel dodał badanieWiFi/BT/modemu i późniejszezero-copy. Bez NVRAM/mostu/sekretów.

> AKTUALNE: v53 nadal bez reboot. Test12h oraz fizyczny reconnectUSB PASS: nowa sesja, control78/blocks0/Charging, errors4 i fault_seen1 stabilne. Poprawiono wykrywanie zakończenia telnetu (fałszywy rc=$? z echa komendy), testy regresji i dłuższy odczytPASS. LoggerPC działa z poprawionym klientem. DE C naPC: prawdziwe warstwy, realne okna, sekwencjafullscreen/restore/minimize/activate/maximize/close i C++API PASS. TelefonUI/sterownikGPU jeszcze nieuruchomione. Wyniki i ograniczenia w docs/NIGHT_WORK_2026-10-25.md oraz phone/ui/README.md.

> Najnowsze: v53 bez restartu przekroczył12h; blokada sesjiPASS (blocks2/control7c/Not charging), watchdog dalej działa i błędy nie rosną. Czekamy na test odłączeniaUSB20s/wznowienia od właściciela. Natywny desktopC naPC zarządza realnymi oknami przez layer-shell/foreign-toplevel na labwc; pełna sekwencja stanów okien i C++ABI PASS. README phone/ui aktualne. GPU: kompilacyjnyBVNC22.49.21.16 znaleziony w stock, nie potwierdzony sprzętowo. Szczegóły/dowody na końcu docs/NIGHT_WORK_2026-10-25.md.

> Aktualizacja po powrocie właściciela 2026-10-25: restart/testy przy telefonie znów DOZWOLONE; od startuv53 nie wykonano kolejnego restartu. Uptime ponad11h, nocne liczniki stabilne; ważna rozbieżność ADC~4.25V vs profilCV4.20V i brakFull wymagają zbadania. UI: pełne DE w C/C++, mała skala CAŁOŚCI, ENGLISH, normalny desktop, gesty/multitouch, fullscreen -> zwykłe okna ze standardowym toolbarem. Kod phone/ui, pomiary i plan docs/NIGHT_WORK_2026-10-25.md. Poniższy zakaz nocny jest historyczny.

> AKTUALNE: v53 POTWIERDZONY (#39), zapis/readback i start PASS. Po ~32s guard przejął domyślny profil ładowarki; przy uptime414s kicks47, errors4 i timer_faults_seen1 stabilne od początku, fault0/temp29.1C. Ograniczenie początkowego profilu LK opisane w docs/MFW_V53_CHARGING.md. WŁAŚCICIEL ODSZEDŁ OD TELEFONU: PRZEZ NOC ŻADNYCH RESTARTÓW/POWEROFF/SUSPEND/RESETÓW, ŻADNEGO ODPINANIA USB. Logger PC mfw_night_monitor.py działa co60s/10h. Praca nocna: sprzęt i nowa powłoka C/Wayland, bez aktywowania nowego kernela; testy rano. Ustalenia w docs/NIGHT_WORK_2026-10-25.md.

> Stan 2026-10-25: v52 POTWIERDZONY (#36), reboot dochodzi sam do LK, potem krótkie Power. V53 ładowanie HL7005D PRZYGOTOWANY, staging bez zapisu boot; potrzebna zgoda według zasady9. Testy natywne watchdoga/CE/TE PASS; nowy kernel jeszcze nietestowany na telefonie. Szczegóły docs/MFW_V53_CHARGING.md; dziennik phone/NOTES.md.

> Najnowszy stan: v52 POTWIERDZONY, test zwykłego reboot z52 PASS — sam przeszedł do LK, potem tylko krótkie Power i powrót #36/uptime27s. TOPRGUpriority192 działa na DURA-L21. Krótki Power na LK nadal konieczny; nie zmieniać LK. Bieżący mfw_v52.img. Zgodnie z planem teraz diagnostyka ładowania HL7005D/watchdog32s. Dowody: koniec docs/MFW_V52_RESTART.md i phone/NOTES.md.

> Najnowszy stan: właściciel zatwierdził i wykonano zapis mfw_v52.img przez USB, pełny readback SHA256 PASS. Wysłano reboot ze starego51; pierwszy ping0/2, czekamy na start52 (możliwy długi Power) i następnie właściwy test restartu z52. Autoryzacja obejmuje ten test. Ładowanie dopiero po jego sukcesie. Szczegóły na końcu docs/MFW_V52_RESTART.md.

> Aktualnie: v51 potwierdzony, mfw_v52.img przygotowany tylko do testu restartu (TOPRGU192 przed PSCI129, warunek DURA-L21). Build/obraz/patch oraz --stage PASS; zapis i test jeszcze niewykonane. Czekamy na zgodę na konkretny zapis v52 według zasad właściciela. Przejście51->52 może wymagać Power; właściwy test dopiero z52. Ładowanie dopiero po sukcesie restartu. Procedura docs/MFW_V52_RESTART.md.

> Aktualny stan 2026-10-25: mfw_v51.img POTWIERDZONY na urządzeniu (kernel #35), po poprawnym zapisie przez USB i ręcznym wymuszeniu restartu. HL7005D zgłasza Charging/online1/fault0 przy pierwszym odczycie, VBAT/NTC działają, CPU odczyt ~950MHz. Restart programowy pozostaje do naprawy; nie pomylić jego zawieszenia z awarią boot v51. Bieżący obraz mfw_v51.img, awaryjny wd_v50.img. Wyniki na końcu docs/MFW_V51_UPDATES.md.

> Najnowszy stan: właściciel zatwierdził zapis v51 przez USB. Backup, zapis boot i pełny readback SHA256 PASS; reboot wysłany. Telefon po restarcie nie odpowiada na ping, Windows nie pokazuje RNDIS. Rozruch v51 NIEPOTWIERDZONY — potrzebne zdjęcie ekranu od właściciela. Nie zgadywać przyczyny; v50 ostatni potwierdzony boot. Szczegóły na końcu docs/MFW_V51_UPDATES.md i phone/NOTES.md.

> Stan 2026-10-25: v50 POTWIERDZONY; nowy mfw_v51.img gotowy do testu (stan HL7005D i read-only CPU ~950MHz). Nowe nazwy mfw_vNN.img. Aktualizator USB przeszedł check/stage, prywatny backup boot24MiB zweryfikowany, ale NIE wykonano zapisu ani restartu — czekamy na zgodę właściciela według jego pierwotnych zasad. Dokładna procedura i ograniczenia: docs/MFW_V51_UPDATES.md.

> Nowsza praca: v50 gotowy do testu właściciela (NTC ~27.7°C), v49 nadal potwierdzony. Ładowarka ma ID0x41 i stock rozpoznaje ją jako HL7005D, pomimo nazwy sterownika fan54005. Szczegóły: docs/PMIC_V50_TEMPERATURE.md.

> **Aktualny stan Codex, 2026-10-24: v49 POTWIERDZONY na telefonie.** Działa konsola, rootfs/USB, świeże napięcie baterii oraz Power/Vol+/Vol− przez evdev. Wyniki: docs/PMIC_V49_TEST.md. v48 zachować awaryjnie. Następny etap: temperatura i ładowarka FAN54005; wcześniejsze opisy kandydatów są historyczne.

# HANDOFF: przekazanie pracy innemu agentowi

Stan: 2026-10-24, wieczór. Poprzedni agent (Claude) kończył limit; praca ma być kontynuowana przez innego agenta
(np. Codex) na tych samych maszynach (2 PC + telefon), a potem wróci Claude. **Wszystko, co trzeba wiedzieć, jest w repo.**
Chronologia, błędne tropy i pomiary: `phone/NOTES.md`.

## 0. Zasady współpracy z właścicielem
- Właściciel mówi po **polsku**, chce **krótkich, konkretnych** instrukcji i **oszczędzania tokenów**.
- Każdy test kernela = **flash przez BROM, który robi właściciel ręcznie** (telefon wyłączony, tryb BROM). Podawaj mu gotową komendę
  i **co dokładnie ma obserwować/sfotografować**. Wyniki widać na ekranie telefonu (zdjęcie ekranu).
- Właściciel odmówił UART. Dostępne kanały: ekran (tekst z dmesg), USB RNDIS (od v41), most.
- Sekretów nie wolno wklejać do czatu. Właściciel trzyma repo jako prywatne i nikogo nie dodaje.
- Nie zgaduj: podawaj, co jest zmierzone, a co hipotezą. Wcześniejsze błędne hipotezy (VA52, rozmiar kernela, czas init) były odwołane.

## 1. Maszyny i ścieżki
**PC z telefonem i WSL** (Windows; agent PC jako zwykły użytkownik, **bez admina**: nie nada IP karcie, nie zainstaluje sterownika):
| Rzecz | Ścieżka |
|---|---|
| Praca, obrazy, narzędzia | `D:\y5_linux_work\` (podkatalog `out\` = binaria: `Image_wd19`, `v9.dtb`, `y5txt`, `y5cal`, `wdt`, `stock_0.dts`) |
| mtkclient (flash BROM) | `D:\mtkclient_work\mtk.py` |
| Kopia `boot` Androida 4.4 (przywracanie) | `D:\y5_backups\20261023_201905\boot.img` |
| Nagłówek stockowego boot (wzorzec dla obrazów) | `D:\huawei_y5_backup\boot.bin` |
| Alpine minirootfs | `D:\y5_linux_work\alpine-minirootfs-3.24.2-aarch64.tar.gz` |
| Ghidra 12.1.4 + JDK21 | `D:\y5_linux_work\ghidra_k44` (projekt), `jdk21\jdk-21.0.12.1+1` |
| Źródła DTB extras (kopia robocza) | `C:\Users\pc\Documents\y5_linux_work\extras_vN.dtsi` (kopie w repo: `phone/stub/`) |
| WSL (distro `archlinux`): kernel | `/home/y5/linux-7.3` (+ `/home/y5/linux-7.3.tar.xz` = czysty), logi `/home/y5/build73*.log` |

Drugi PC: nginx + Cloudflare (most). Szczegóły: `docs/KLUCZE_I_ZMIENNE.md`.

## 2. Jak sterować (most)
```
python3 bridge/y5sh_cloud.py ps "Get-Date"                     # PowerShell na PC
python3 bridge/y5sh_cloud.py pc-put LOKALNY 'D:\zdalna\sciezka'  # plik do PC
python3 bridge/y5sh_cloud.py pc-get 'D:\zdalna' LOKALNY          # plik z PC
python3 bridge/y5sh_cloud.py run "uname -a"                      # polecenie na telefonie (tylko gdy dziala Android/most telefonu)
```
Zmienne: `PC_URL`, `BRIDGE_PS_TOKEN`, `BRIDGE_PS_SIGN_KEY` (+ `BRIDGE_URL`, `BRIDGE_TOKEN`, `BRIDGE_SIGN_KEY`). Most PC działa tylko gdy właściciel
uruchomił `start_bridge_signed.ps1 -Pc -PcHours N` (wygasa; `not found` = wygasł lub jest plik `DISABLE_PC`).

Pułapki mostu:
- Limit Cloudflare **100 s** na żądanie; domyślny timeout `ps` ok. 60 s. Długie rzeczy: `nohup` w WSL + log + odpytywanie.
- **Zagnieżdżone cudzysłowy psują się.** Wzorzec, który działa: napisz skrypt `.sh`/`.ps1` lokalnie, `pc-put` na PC, potem
  `ps 'wsl -d archlinux -- bash /mnt/d/y5_linux_work/skrypt.sh'`. Nie używaj `sed -i s/\r//` na własnych plikach (wycina literę `r`).
- Zdalne `Start-Sleep` > 60 s zabija żądanie.

## 3. Pętla pracy nad kernelem
Szczegóły i komendy: `docs/BUILD_AND_FLASH.md`. W skrócie: zmiana w `/home/y5/linux-7.3` -> `bld.sh` (nohup) -> kopia `Image` do `out\Image_wdNN` ->
`bm_wdNN.py` składa obraz boot (kernel gz + DTB + Alpine initramfs) -> właściciel: `python mtk.py w boot ...` -> obserwacja ekranu/RNDIS.

## 4. Odzyskiwanie
- Telefon po złym obrazie: BROM -> `python mtk.py w boot D:\y5_backups\20261023_201905\boot.img` (wraca Android 4.4).
- Preloader i LK nie są ruszane; BROM zawsze działa. LK **nie weryfikuje zawartości kernela** w `boot` (recovery jest weryfikowane: testuj tylko `boot`).
- Czerwony ekran LK „Your device has failed verification” przy starcie jest normalny (Power = kontynuuj; samo też przechodzi).

## 5. Ustalone fakty sprzętowe (wszystko zmierzone)
**Rozruch.** LK honoruje `kernel_addr=0x40200000`; ramdisk `0x45000000` (z nagłówka, ładuje LK), tags/DTB `0x44000000`. Obraz to `Image.gz-dtb`.
Kernel ≤ ok. 12,7 MB nieskompresowany działa; wbudowanie 16,5 MB initramfs w kernel **zepsuło** rozruch (ramdisk dawać z LK).
**Przerwania.** Root `interrupt-parent` = `sysirq` (phandle 1, `mt6577-sysirq`, rodzic `gic` phandle 2). Urządzenia: `<0 N 8>` (level-low 8).
**Watchdog TOPRGU** `0x10007000`: MODE(0x0), LENGTH(0x4 = `(T<<11)|8`, pole 11-bitowe, **T ≤ 31 s**), RESTART(0x8 = 0x1971). Wyłączenie: zapis `0x22000000` do MODE
(init robi to narzędziem `wdt x 0x10007000 0x22000000`). Sterownik `mtk-wdt` w kernelu włącza 30 s: docelowo karmić albo wyłączać świadomie.
**Ekran** (panel 720x1440, DSI, wymaga ręcznego wyzwolenia odświeżania):
- Skanowany bufor: baza empiryczna `0x775E8E38`, **skok wiersza 720 px (2880 B)**, piksel XRGB8888 (0xFFRRGGBB). Pierwszy piksel (0,0) nie jest rysowany (efekt toru wyświetlania). LK ma bufor `0x771E0000–0x77FFFFFF`.
- Wyzwolenie po zapisie: `0x14015020=1` (MUTEX0 EN), `0x1400B010=1` (RDMA0), `0x14012000`: zapis 0 potem 1 (DSI0 START). Sam DSI-start bez mutex/RDMA tylko miga starym obrazem.
- Kernelowy `simpledrm`/fbcon **jeszcze nie rysuje na panelu** (węzeł `simple-framebuffer` w DTB ma zły adres/stride 2944, a nikt nie wyzwala odświeżania). Dlatego `y5txt` (patrz niżej).
**USB.** t-phy v1: rodzic `0x11CC0000` (`reg` size 0x100, **nie 0x1000**: kolidowało z dzieckiem), dziecko `usb-phy@0x11CC0800`; MUSB `0x11200000` (SPI 73), tryb `peripheral`,
**`CONFIG_MUSB_PIO_ONLY=y`** (bez tego `DMA controller not set` -> -ENODEV). Zegary (`infra_icusb`, `usb_top_sel`) ungate'owane przez hak w `init/main.c`
(`y5_clk_hack`), bo mainline nie ma sterownika zegarów MT6739; w DTB są `fixed-clock`. Gadżet: configfs (`g1`, VID 1d6b:0104, RNDIS + MS OS descriptors), bez tego Windows
przypina `usbser` (COM). Detekcja VBUS/ładowarki przez PMIC **nie jest podpięta**, ale gadżet i tak enumeruje.
**eMMC.** `mediatek,mt8183-mmc` @`0x11230000` (+ top `0x11CD0000`, SPI 79), `bus-width 8`, HS200; wymaga `CONFIG_REGULATOR_FIXED_VOLTAGE=y` (węzły `regulator-fixed`
w DTB, inaczej `vmmc regulator not available`). GPT MediaTek jest niestandardowy (ostrzeżenia `first_usable_lba`/`crc32`), ale p1..p18 się wykrywają.
**Dane z 4.4** (Ghidra + kallsyms): `phone/infra_gates_44.txt`, `phone/top_muxes_44.txt`, `phone/kallsyms_44.txt`, `phone/ghidra/` (skrypt `Y5Decomp.java`, wyniki `k44_dec*.c`).
Struktury: `mtk_gate` krok 0x30 (id@0, name@8, parent@0x10, regs ptr@0x18 {sta,clr,set}, bit@0x20); `mux` krok 0x30 (ofs @0x18..0x24, shift/width/gate/updsh @0x28..0x2b).

## 6. Narzędzia userlandowe na telefonie (`phone/stub/`, w initramfs)
`wdt` (`wdt3.c`: `wdt T`, `wdt r ADDR`, `wdt w ADDR VAL` bajt, `wdt x ADDR VAL` 32-bit, `wdt fill ADDR LEN VAL`), `y5txt` (tekst z pliku na panel), `y5cal` (wzór kalibracyjny / jednolity kolor).
Kompilacja: `aarch64-linux-gnu-gcc -static -nostdlib -O2 -ffreestanding -fno-tree-loop-distribute-patterns` (`build_tools.sh`). Czcionka: `mkfont.py` (wycina komentarze z `font_8x16.c`; assert 4096 bajtów).

## 7. Pułapki (każda kosztowała cykl BROM)
1. **`make olddefconfig` zawsze z `ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-`.** Bez tego `.config` traci symbole arm64.
2. **Własny `memset` w `-nostdlib`**: GCC zamienia pętlę na wywołanie samego siebie (nieskończona pętla). Flagi z pkt 6 + sprawdzaj `objdump`.
3. Regex po tablicy czcionki łapał `0x..` z komentarzy -> glify przesunięte. Naprawione w `mkfont.py`.
4. Watchdog: LENGTH 11-bitowy; wartości > 31 s zawijają. Model „restart ≈ 2×T” zawodził; kanał czasowy uznany za niewiarygodny. Używaj ekranu.
5. `devices_deferred` wymaga `CONFIG_DEBUG_FS` i może się zawiesić (blokada): czytaj w tle, nigdy w pętli głównej.
6. Pętla init, która blokuje się na odczycie z `sysfs/debugfs`, nie dojdzie do rysowania: wszystko ryzykowne w tle.
7. Brak klawiatury/dotyku na 7.3: sterowanie tylko przez ekran, USB i most.
8. Windows: agent PC bez admina (nie nada IP, nie zainstaluje sterownika). Dlatego DHCP na telefonie i MS OS descriptors.
9. Internet właściciela czasem zanika na ~10 s (most chwilowo niedostępny): ponów.

## 8. Co działa w obrazie v41 (`phone/stub/bm_wd41.py`)
Init: montuje devtmpfs/proc/sysfs/debugfs, wyłącza watchdog, tworzy gadżet RNDIS (configfs), `usb0=192.168.7.2/24`, `udhcpd` (pula 192.168.7.10–20 dla PC),
`telnetd -b 192.168.7.2 -l /bin/sh` (**TYLKO TEST, bez hasła**), a na ekranie pętla diagnostyczna (filtr dmesg, stan UDC/MMC/MUSB, driver bindings).
Bezpieczeństwo: telnet dostępny tylko na łączu USB, ale to otwarty root dla każdego procesu na tym PC. **Zamienić na SSH z kluczem (dropbear) jak najszybciej.**

## 9. Kolejne kroki (kolejność proponowana)
1. **Zweryfikować v41:** karta RNDIS na PC dostaje `192.168.7.x` (DHCP), `ping 192.168.7.2`, połączenie z `telnetd` (Python 3.13 na PC nie ma `telnetlib`: napisać prosty klient na gnieździe, obsłużyć IAC) i zrobić z tego narzędzie `tools/phone_shell.py`.
2. **Partycje:** `cat /proc/partitions`, nazwy z `/sys/class/block/mmcblk0pN/uevent` (`PARTNAME`). Wybrać `userdata` (duża) na rootfs; **nie ruszać** `nvdata/nvram/nvcfg/proinfo/protect*/boot*/lk/preloader`.
3. **Rootfs na flash:** `mkfs.ext4` (lub f2fs) na `userdata`, rozpakować Alpine, init w initramfs montuje i robi `switch_root`. Potem OpenRC (domyślny w Alpine) lub systemd.
4. **Konsola graficzna z logiem startu:** poprawić `simple-framebuffer` w DTB (adres `0x775E8E38`, stride 2880, format), dodać w kernelu wątek/timer wyzwalający odświeżanie panelu (mutex/RDMA/DSI jak w pkt 5), `console=tty0`. Wtedy log boot i `[ OK ]` z init widać na ekranie; `y5txt` do wyrzucenia.
5. **Sprzątanie:** usunąć hooki diagnostyczne (`y5cp*`, checkpointy przez watchdog, `Y5A` w `mediatek.c`), przenieść `extras_v9.dtsi` do właściwego `mt6739.dtsi` (`archive/pc_work/kernel/mt6739.dtsi` to wcześniejszy źródłowy plik DTS), przygotować czysty patchset.
6. **Sterownik zegarów `clk-mt6739`** z tablic 4.4 (zamiast `y5_clk_hack` i fixed-clock), regulatory i PMIC (zasilanie, ładowanie, detekcja VBUS).
7. **DMA MUSB** (Inventra HSDMA) zamiast PIO.
8. **Sterowniki** (porty z 4.4 przez Ghidrę, jeśli brak mainline): dotyk, LED, latarka, IMU/czujniki, podświetlenie, **panel DSI jako prawdziwy DRM**, audio, GPS, **modem** (CCCI/`nvram_daemon`: patrz `phone/NOTES.md` sekcja „Modem”; uwaga na zapis NVRAM).
9. **UI w C** (wlroots + pixman, rysowanie CPU wystarcza: pomiary w `phone/NOTES.md`), styl iOS, dynamic notch na końcu. Założenia właściciela: `phone/NOTES.md` -> „Decyzje użytkownika”.

## 10. Do zrobienia przez właściciela / higiena
- Odwołać 2 klucze Tailscale (wcześniej użyte). Rozważyć rotację tokenów mostu.
- Zamienić telnet na SSH z kluczem (decyzja właściciela: telnet tylko do testu).
- Ewentualnie zaszyfrowana kopia kluczy w repo: `tools/pack_secrets.ps1` (hasło zna tylko właściciel).

## Aktualizacja przekazania: Codex lokalnie, 2026-10-24
Bieżący stan według właściciela: wd_v48.img, Image_wd22, v11.dtb; działają rootfs z flash i konsola graficzna z banerem. Starsze sekcje tego dokumentu opisują v41. Pomiar Codexa: ping 2/2 <1 ms, powłoka działa, Linux 7.3.0 #30, hostname microfw. Nie potwierdzono numeru obrazu przez uname. Szczegóły: phone/NOTES.md, sekcja „Przejęcie lokalne przez Codex”. Pracować lokalnie bez mostu i bez odczytu secrets. Nie wykonywać starych instrukcji zapisu logo/expdb; temat własnej partycji logo zamknięty przez właściciela. Najbliższy krok: SSH z kluczem zamiast testowego telnetu.

Zmiana priorytetu właściciela: Dropbear odłożony; celem bieżących prac jest używalność jako drugie urządzenie na co dzień. Najbliższy kierunek: identyfikacja i uruchomienie dotyku; szczegóły pomiaru w phone/NOTES.md.


Najnowszy priorytet właściciela: PMIC, bateria, przyciski (przed dotykiem/UI). Stan rozpoznania: docs/PMIC_BATTERY_BUTTONS.md; źródła robocze w D:\y5_linux_work.


Kandydat v49 gotowy (jeszcze bez testu BROM): MT6357 potwierdzony 0x5721, świeże VBAT ~3.87V, pomiarami właściciela zidentyfikowane Power/Vol+/Vol-. Nowy sterownik polling/sysfs/input, Image_wd23/v12.dtb. v48 nadal potwierdzony. Dokładne pliki, SHA256 i test: docs/PMIC_BATTERY_BUTTONS.md. Pełny alternatywny patch: phone/kernel/y5-kernel-7.3-pmic.patch (14 plików).
