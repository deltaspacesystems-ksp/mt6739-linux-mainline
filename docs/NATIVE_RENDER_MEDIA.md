# Native rendering, zero-copy and media acceleration

## Measured on v62/#48, 2026-10-26

Native GPU firmware/CONNECT, CPU PMR mapping and now GPU virtual-address mapping all pass. Exact generated DDK1.9 ABI creates a context/heap, reserves and maps4096bytes at0x65c0000000, then releases the entire dependency chain. Driver/FirmwareOK,HWR0,CRR0,APM0,VCORE1.15V,Charging after execution. This is GPU page-table setup, not shader execution or rendering. Native client --gpu-memory and22 host mock scenarios use actual generated1.9 types; ABI staticasserts and ASan/UBSan PASS. Failed VM teardown stops parent/PMR release and leaves remaining handles to DISCONNECT. Evidence phone/hardware/gpu-va-v62.json.

## Rendering and display

Next userspace requirement: port Mesa pvrsrvkm winsys1.17 structures/commands/synchronization to exact1.9 and provide measured22.49.21.16/GE8100 compiler/tile capabilities. No borrowed GE8300 profile and no Android CPU runtime. Mapping a buffer is necessary but insufficient for a render job.

The local DDK services/server/env/linux/physmem_dmabuf.c explicitly rejects exported DMA-BUF external attach with ENOSYS and map with EINVAL. Import support is separate; this audit does not prove it works. Candidate zero-copy direction: native display DRM allocates/exports scanout-capable GEM/DMA-BUF, GPU imports and renders into it; KMS pageflip/vblank/fences synchronize presentation. Requires real display DRM owning the measured OVL3/directlink RDMA/continuous DSI pipeline; existing simpledrm plus fixed LK framebuffer is insufficient. Existing v58 physical-buffer flip is an earlier proof, not a GEM/KMS interface. Do not remove the export guard without implementing complete SG/DMA/lifetime/synchronization handling.

## Media engines: live DT inventory

| Node | Base/size | IRQ SPI (DT flags8) | Compatible |
|---|---|---|---|
| vdec | 0x17040000/0x10000 |215| mediatek,vdec |
| vdec_gcon |0x17000000/0x1000| none in node |mediatek,vdec_gcon|
| venc |0x17020000/0x1000|212|mediatek,venc|
| venc_global_con |0x17000000/0x10000|none in node|mediatek,venc_global_con; syscon|
| venc_jpg |0x17030000/0x1000|214|mediatek,venc_jpg|
| seninf |0x15008000/0x1000|none in node|mediatek,seninf|
| camsv |0x15009000/0x1000|220|mediatek,camsv|
| audio |0x11220000/0x1000|none printed|mediatek,audio; syscon|

DT flags8 are level-low; IRQ numbers are SPI specifiers, not Linux IRQs. Two global-con nodes overlap; do not bind unrelated independent owners. vdec_gcon names MT_CG_VDEC/MT_CG_VENC/MT_SCP_SYS_VDE/MT_SCP_SYS_VEN (clock tuples phandle0x37 indices3/1 and0x1a index6 twice). These are stock DT facts, not validated Linux power-domain assignments. No engine register accessed or powered in this audit.

Local mainline Mediatek vcodec sources list MT8173/8183/8192/8186/8195/8188 decoder compatibles and no exact MT6739 match. Existing H264/HEVC code therefore does not establish compatibility. Stock symbol table contains vcodec_lockhw/waitisr/mmap/unlocked_ioctl and eVideo* helpers: this suggests an older vendor control ABI, but is not proof of complete decoder operation or a sufficient native userspace implementation. Next compare exact GPL stock ioctl/register programming and firmware responsibilities before selecting an adaptation. Hardware decode must be validated separately for H264 and HEVC with known clips and decoded-frame checks; GPU firmware success proves neither.

## Audio

ALSA/ASoC framework modules load. MT6357 codec source exists, but no active AFE/DMA/machine routing or physical PCM device. Need native PMIC regmap/MFD ownership compatible with the working charger, MT6739 AFE/ADDA/clocks/DMA, routing and physical mic/speaker identification. No microphone capture or speaker playback has been attempted.

Owner currently away. Continue source/module work without assuming visual/physical validation. Active-GPU kexec handover remains untested.


## 2026-10-27: native codec interface audit
Rechecked all7 downloaded MT6739 reference files against recorded SHA256:PASS. Source index finds20 declared ioctl names with case labels and17 without; this is lexical source evidence, not compiled/runtime coverage. The character driver exposes MMIO mapping, memory allocation, lock/unlock, waitISR and power/accounting, not a complete frame-level V4L2 decoder. Its hal.c command queue/power/ISR cases are stubs returning success; they cannot decode video. H264_DEC/HEVC_DEC share control branches; generic HEVC_ENC enum references do not prove physical encoder support. Decoder clocks are enabled SCP_SYS_VDE then CG_VDEC; do not activate until native domains/clock ownership and register programming are established.
A straight ioctl port is insufficient: native bitstream parsing/register programming (or a verified compatible open backend), DMA/cache/IOMMU and V4L2 buffers/controls are still required. Legacy mmap start/length checks must not be copied unchecked; validate the full span and allocation ownership. No engine powered or register accessed. Evidence phone/hardware/vcodec-native-interface-audit.json; reproducible script archive/pc_work/audit_vcodec_interface.py.


## 2026-10-27: interrupt protocol comparison
MT6739 reference decoder uses MISC at engine+0x5000, IRQ register MISC+0xa4, done bit16, then OR0x11/write and AND~0x10/write. Linux7.3 mainline decoder uses the same acknowledgement fields. This is a useful narrow protocol match, not proof that the entire engine/firmware is compatible. The mainline legacy H264 backend calls vpu_dec_init/start/reset; the downloaded stock control driver does not supply that protocol. Do not add an MT6739 compatible solely on the IRQ match.
Stock non-CLKMGR power order also requires SMI_LARB_VENCSYS before SCP_SYS_VDE/CG_VDEC (or VEN/VENC), then reverse on shutdown. Its failure handling logs errors and continues; a native port must unwind completed stages and balance references. No clocks, power, IRQ handler or MMIO were changed. Evidence phone/hardware/vcodec-irq-power-comparison.json.


## 2026-10-27: exact Huawei clock consumer correlation
Actual stock vcodec_probe assigns globals9040/9048/9050/9058 to MT_CG_VDEC/MT_CG_VENC/MT_SCP_SYS_VDE/MT_SCP_SYS_VEN (full addresses in evidence JSON). Bounded ASCII reads of the kernel strings confirm these names, rather than inferring from the Wiko reference. Combined with the stock power functions, decoder enable order is SMI bus1, SCP_SYS_VDE, CG_VDEC; disable reverses clocks and then SMI. These are old consumer names, not verified mainline clock-provider IDs or a safe sequence to execute with present providers. No engine activated.
