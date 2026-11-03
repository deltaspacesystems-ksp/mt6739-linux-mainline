/* SPDX-License-Identifier: GPL-2.0-only */
/* Native Linux userspace probe for the exact, generated DDK 1.9 bridge ABI.
 * No Bionic, HAL, vendor userspace library or GPU firmware job submission.
 * Generated headers retain their original Imagination licenses separately.
 */
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#ifndef __user
#define __user /* Raw kernel UAPI header; headers_install strips this annotation. */
#endif
#include <drm.h>
#include "common_srvcore_bridge.h"
#include "common_mm_bridge.h"
#include "pvrversion.h"
#include "rgx_options.h"

struct mfw_bridge_cmd {
    uint32_t bridge_id, function_id;
    uint64_t input, output;
    uint32_t input_size, output_size;
};
#define MFW_BRIDGE_IOCTL _IOWR('d', DRM_COMMAND_BASE, struct mfw_bridge_cmd)
_Static_assert(sizeof(struct mfw_bridge_cmd) == 32, "DRM envelope ABI");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_CONNECT) == 16, "DDK 1.9 input ABI");
_Static_assert(offsetof(PVRSRV_BRIDGE_IN_CONNECT, ui32Flags) == 0, "flags offset");
_Static_assert(offsetof(PVRSRV_BRIDGE_IN_CONNECT, ui32ClientDDKBuild) == 12, "build offset");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_CONNECT) == 17, "DDK 1.9 output ABI");
_Static_assert(offsetof(PVRSRV_BRIDGE_OUT_CONNECT, eError) == 13, "error offset");
_Static_assert(PVRSRV_BRIDGE_MM_HEAPCFGHEAPCOUNT == 28, "1.9 heap-count command");
_Static_assert(PVRSRV_BRIDGE_MM_HEAPCFGHEAPDETAILS == 30, "1.9 heap-details command");
_Static_assert(offsetof(PVRSRV_BRIDGE_OUT_HEAPCFGHEAPCOUNT, eError) == 4, "heap-count error offset");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_HEAPCFGHEAPDETAILS) == 20, "heap-details input ABI");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_HEAPCFGHEAPDETAILS) == 40, "heap-details output ABI");
_Static_assert(offsetof(PVRSRV_BRIDGE_OUT_HEAPCFGHEAPDETAILS, eError) == 36, "heap-details error offset");
_Static_assert(sizeof(PVRSRV_MEMALLOCFLAGS_T) == 4, "DDK1.9 memory flags are 32-bit");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_PHYSMEMNEWRAMBACKEDLOCKEDPMR) == 52, "PMR allocation input ABI");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_PHYSMEMNEWRAMBACKEDLOCKEDPMR) == 12, "PMR allocation output ABI");

static int call_bridge(int fd, uint32_t bridge, uint32_t function, void *input, uint32_t input_size,
                       void *output, uint32_t output_size)
{
    struct mfw_bridge_cmd cmd = {
        .bridge_id = bridge,
        .function_id = function,
        .input = (uintptr_t)input, .output = (uintptr_t)output,
        .input_size = input_size, .output_size = output_size,
    };
    return ioctl(fd, MFW_BRIDGE_IOCTL, &cmd);
}

/* Native DDK1.9 GPU VA lifecycle; no rendering submission. */
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_DEVMEMINTCTXCREATE)==4, "context input");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_DEVMEMINTCTXCREATE)==24, "context output");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_DEVMEMINTHEAPCREATE)==28, "heap input");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_DEVMEMINTRESERVERANGE)==24, "reserve input");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_DEVMEMINTMAPPMR)==28, "map input");
static int gpu_va_roundtrip(int fd, IMG_HANDLE pmr)
{
 char name[128]={0};
 PVRSRV_BRIDGE_IN_HEAPCFGHEAPDETAILS query={.ui32HeapConfigIndex=0,.ui32HeapIndex=1,.ui32HeapNameBufSz=sizeof(name),.puiHeapNameOut=name};
 PVRSRV_BRIDGE_OUT_HEAPCFGHEAPDETAILS detail={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
 PVRSRV_BRIDGE_IN_DEVMEMINTCTXCREATE ci={.bbKernelMemoryCtx=IMG_FALSE};
 PVRSRV_BRIDGE_OUT_DEVMEMINTCTXCREATE co={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
 PVRSRV_BRIDGE_IN_DEVMEMINTHEAPCREATE hi={0};
 PVRSRV_BRIDGE_OUT_DEVMEMINTHEAPCREATE ho={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
 PVRSRV_BRIDGE_IN_DEVMEMINTRESERVERANGE ri={0};
 PVRSRV_BRIDGE_OUT_DEVMEMINTRESERVERANGE ro={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
 PVRSRV_BRIDGE_IN_DEVMEMINTMAPPMR mi={0};
 PVRSRV_BRIDGE_OUT_DEVMEMINTMAPPMR mo={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
 IMG_HANDLE ctx=NULL,heap=NULL,reservation=NULL,mapping=NULL;
 int rc=1;
#define VM_CALL(command,input,output) \
 do { if(call_bridge(fd,6,PVRSRV_BRIDGE_MM_##command,&(input),sizeof(input),&(output),sizeof(output)) || (output).eError!=PVRSRV_OK) { \
 fprintf(stderr,"VM " #command " error=%d\n",(output).eError);goto cleanup;} } while(0)
 VM_CALL(HEAPCFGHEAPDETAILS,query,detail);
 name[127]=0;
 if(strcmp(name,"General") || detail.ui32Log2DataPageSizeOut!=12 || detail.uiHeapLength<4096 || (detail.sDevVAddrBase.uiAddr&4095))goto cleanup;
 VM_CALL(DEVMEMINTCTXCREATE,ci,co);
 if(!co.hDevMemServerContext)goto cleanup;
 ctx=co.hDevMemServerContext;
 hi.hDevmemCtx=ctx;hi.sHeapBaseAddr=detail.sDevVAddrBase;hi.uiHeapLength=detail.uiHeapLength;hi.ui32Log2DataPageSize=12;
 VM_CALL(DEVMEMINTHEAPCREATE,hi,ho);
 if(!ho.hDevmemHeapPtr)goto cleanup;
 heap=ho.hDevmemHeapPtr;
 ri.hDevmemServerHeap=heap;ri.sAddress=detail.sDevVAddrBase;ri.uiLength=4096;
 VM_CALL(DEVMEMINTRESERVERANGE,ri,ro);
 if(!ro.hReservation)goto cleanup;
 reservation=ro.hReservation;
 mi.hDevmemServerHeap=heap;mi.hReservation=reservation;mi.hPMR=pmr;
 mi.uiMapFlags=PVRSRV_MEMALLOCFLAG_GPU_READABLE|PVRSRV_MEMALLOCFLAG_GPU_WRITEABLE|PVRSRV_MEMALLOCFLAG_GPU_CACHE_INCOHERENT;
 VM_CALL(DEVMEMINTMAPPMR,mi,mo);
 if(!mo.hMapping)goto cleanup;
 mapping=mo.hMapping;rc=0;
cleanup:
 /* If teardown fails, stop releasing parents; DISCONNECT owns remaining handles. */
 if(mapping){
  PVRSRV_BRIDGE_IN_DEVMEMINTUNMAPPMR in={.hMapping=mapping};
  PVRSRV_BRIDGE_OUT_DEVMEMINTUNMAPPMR out={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
  if(call_bridge(fd,6,PVRSRV_BRIDGE_MM_DEVMEMINTUNMAPPMR,&in,sizeof(in),&out,sizeof(out))||out.eError!=PVRSRV_OK)return 2;
 }
 if(reservation){
  PVRSRV_BRIDGE_IN_DEVMEMINTUNRESERVERANGE in={.hReservation=reservation};
  PVRSRV_BRIDGE_OUT_DEVMEMINTUNRESERVERANGE out={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
  if(call_bridge(fd,6,PVRSRV_BRIDGE_MM_DEVMEMINTUNRESERVERANGE,&in,sizeof(in),&out,sizeof(out))||out.eError!=PVRSRV_OK)return 2;
 }
 if(heap){
  PVRSRV_BRIDGE_IN_DEVMEMINTHEAPDESTROY in={.hDevmemHeap=heap};
  PVRSRV_BRIDGE_OUT_DEVMEMINTHEAPDESTROY out={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
  if(call_bridge(fd,6,PVRSRV_BRIDGE_MM_DEVMEMINTHEAPDESTROY,&in,sizeof(in),&out,sizeof(out))||out.eError!=PVRSRV_OK)return 2;
 }
 if(ctx){
  PVRSRV_BRIDGE_IN_DEVMEMINTCTXDESTROY in={.hDevmemServerContext=ctx};
  PVRSRV_BRIDGE_OUT_DEVMEMINTCTXDESTROY out={.eError=PVRSRV_ERROR_BRIDGE_CALL_FAILED};
  if(call_bridge(fd,6,PVRSRV_BRIDGE_MM_DEVMEMINTCTXDESTROY,&in,sizeof(in),&out,sizeof(out))||out.eError!=PVRSRV_OK)return 2;
 }
#undef VM_CALL
 if(!rc)printf("GPU_VA_MAP_PASS bytes=4096 address=%llx; mapped/unmapped, no rendering job\n",(unsigned long long)detail.sDevVAddrBase.uiAddr);
 return rc;
}

static int memory_roundtrip(int fd, int gpu_va)
{
    uint32_t mapping = 0, i;
    const char annotation[] = "MICROFW native CPU mapping test";
    PVRSRV_BRIDGE_IN_PHYSMEMNEWRAMBACKEDLOCKEDPMR input = {
        .uiSize = 4096, .uiChunkSize = 4096,
        .ui32NumPhysChunks = 1, .ui32NumVirtChunks = 1,
        .pui32MappingTable = &mapping, .ui32Log2PageSize = 12,
        .uiFlags = PVRSRV_MEMALLOCFLAG_GPU_READABLE | PVRSRV_MEMALLOCFLAG_GPU_WRITEABLE |
                   PVRSRV_MEMALLOCFLAG_CPU_READABLE | PVRSRV_MEMALLOCFLAG_CPU_WRITEABLE |
                   PVRSRV_MEMALLOCFLAG_CPU_WRITE_COMBINE | PVRSRV_MEMALLOCFLAG_GPU_CACHE_INCOHERENT |
                   PVRSRV_MEMALLOCFLAG_KERNEL_CPU_MAPPABLE | PVRSRV_MEMALLOCFLAG_ZERO_ON_ALLOC,
        .ui32AnnotationLength = sizeof(annotation), .puiAnnotation = annotation,
    };
    PVRSRV_BRIDGE_OUT_PHYSMEMNEWRAMBACKEDLOCKEDPMR output = { .eError = PVRSRV_ERROR_BRIDGE_CALL_FAILED };
    PVRSRV_BRIDGE_IN_PMRUNREFUNLOCKPMR release;
    PVRSRV_BRIDGE_OUT_PMRUNREFUNLOCKPMR released = { .eError = PVRSRV_ERROR_BRIDGE_CALL_FAILED };
    volatile uint32_t *memory = MAP_FAILED;
    uintptr_t handle;
    int rc = 1, retain_pmr = 0;
    if (sysconf(_SC_PAGESIZE) != 4096 || sizeof(off_t) != 8) {
        fprintf(stderr, "This trial requires 4K pages and 64-bit offsets\n"); return 1;
    }
    if (call_bridge(fd, 6, PVRSRV_BRIDGE_MM_PHYSMEMNEWRAMBACKEDLOCKEDPMR,
                    &input, sizeof(input), &output, sizeof(output))) {
        perror("ALLOC PMR ioctl"); return 1;
    }
    if (output.eError != PVRSRV_OK || !output.hPMRPtr) {
        fprintf(stderr, "ALLOC PMR error=%d\n", output.eError); return 1;
    }
    handle = (uintptr_t)output.hPMRPtr;
    if (handle > INT64_MAX / 4096) goto release;
    memory = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)(handle * 4096));
    if (memory == MAP_FAILED) { perror("PMR mmap"); goto release; }
    for (i = 0; i < 1024; i++) {
        if (memory[i] != 0) { fprintf(stderr, "PMR initial zero mismatch at %u\n", i); goto unmap; }
    }
    for (i = 0; i < 1024; i++) memory[i] = 0x4d465700U ^ (i * 0x01010101U);
    __sync_synchronize();
    for (i = 0; i < 1024; i++) {
        if (memory[i] != (0x4d465700U ^ (i * 0x01010101U))) {
            fprintf(stderr, "PMR CPU mapping mismatch at %u\n", i); goto unmap;
        }
    }
    rc = gpu_va ? gpu_va_roundtrip(fd, output.hPMRPtr) : 0;
    retain_pmr = rc == 2;
unmap:
    if (munmap((void *)memory, 4096)) { perror("PMR munmap"); rc = 1; }
release:
    if (retain_pmr) return 1; /* DISCONNECT releases dependent handles safely. */
    release.hPMR = output.hPMRPtr;
    if (call_bridge(fd, 6, PVRSRV_BRIDGE_MM_PMRUNREFUNLOCKPMR,
                    &release, sizeof(release), &released, sizeof(released))) {
        perror("FREE PMR ioctl"); rc = 1;
    } else if (released.eError != PVRSRV_OK) {
        fprintf(stderr, "FREE PMR error=%d\n", released.eError); rc = 1;
    }
    if (!rc) puts("PMR_CPU_MAP_PASS bytes=4096 zero/write/read/unmap/free; no GPU rendering job");
    return rc;
}

static int dump_heaps(int fd)
{
    PVRSRV_BRIDGE_IN_HEAPCFGHEAPCOUNT in = { .ui32HeapConfigIndex = 0 };
    PVRSRV_BRIDGE_OUT_HEAPCFGHEAPCOUNT out = { .eError = PVRSRV_ERROR_BRIDGE_CALL_FAILED };
    uint32_t i;
    if (call_bridge(fd, 6, PVRSRV_BRIDGE_MM_HEAPCFGHEAPCOUNT, &in, sizeof(in), &out, sizeof(out))) {
        perror("HEAPCOUNT ioctl"); return 1;
    }
    if (out.eError != PVRSRV_OK || !out.ui32NumHeaps || out.ui32NumHeaps > 64) {
        fprintf(stderr, "Invalid heap count: error=%d count=%u\n", out.eError, out.ui32NumHeaps);
        return 1;
    }
    printf("HEAPS count=%u\n", out.ui32NumHeaps);
    for (i = 0; i < out.ui32NumHeaps; i++) {
        char name[128] = { 0 };
        PVRSRV_BRIDGE_IN_HEAPCFGHEAPDETAILS detail = {
            .ui32HeapConfigIndex = 0, .ui32HeapIndex = i,
            .ui32HeapNameBufSz = sizeof(name), .puiHeapNameOut = name,
        };
        PVRSRV_BRIDGE_OUT_HEAPCFGHEAPDETAILS result = { .eError = PVRSRV_ERROR_BRIDGE_CALL_FAILED };
        if (call_bridge(fd, 6, PVRSRV_BRIDGE_MM_HEAPCFGHEAPDETAILS, &detail, sizeof(detail), &result, sizeof(result))) {
            perror("HEAPDETAILS ioctl"); return 1;
        }
        if (result.eError != PVRSRV_OK) {
            fprintf(stderr, "HEAPDETAILS error=%d index=%u\n", result.eError, i); return 1;
        }
        name[sizeof(name)-1] = 0;
        printf("HEAP %u %s base=%llx size=%llx page_log2=%u align_log2=%u tile_log2=%u\n",
               i, name, (unsigned long long)result.sDevVAddrBase.uiAddr,
               (unsigned long long)result.uiHeapLength, result.ui32Log2DataPageSizeOut,
               result.ui32Log2ImportAlignmentOut, result.ui32Log2TilingStrideFactorOut);
    }
    return 0;
}

int main(int argc, char **argv)
{
    int fd, rc = 1, query_rc = 0;
    char name[64] = { 0 };
    struct drm_version version = { .name_len = sizeof(name) - 1, .name = name };
    PVRSRV_BRIDGE_IN_CONNECT input = {
        .ui32Flags = 1U << 5, /* SRV_FLAGS_CLIENT_64BIT_COMPAT */
        .ui32ClientBuildOptions = RGX_BUILD_OPTIONS,
        .ui32ClientDDKVersion = PVRVERSION_PACK(PVRVERSION_MAJ, PVRVERSION_MIN),
        .ui32ClientDDKBuild = PVRVERSION_BUILD,
    };
    PVRSRV_BRIDGE_OUT_CONNECT output = { .eError = PVRSRV_ERROR_BRIDGE_CALL_FAILED };
    PVRSRV_BRIDGE_IN_DISCONNECT disconnect = { 0 };
    PVRSRV_BRIDGE_OUT_DISCONNECT disconnected = { .eError = PVRSRV_ERROR_BRIDGE_CALL_FAILED };

    if (argc == 1 || (argc == 2 && !strcmp(argv[1], "--abi"))) {
        printf("DDK=%u.%u@%u connect_in=%zu connect_out=%zu error_offset=%zu build_options=%08x\n",
               PVRVERSION_MAJ, PVRVERSION_MIN, PVRVERSION_BUILD,
               sizeof(input), sizeof(output), offsetof(PVRSRV_BRIDGE_OUT_CONNECT, eError),
               (unsigned int)RGX_BUILD_OPTIONS);
        return 0;
    }
    if (argc != 3 || (strcmp(argv[1], "--connect") && strcmp(argv[1], "--heaps") && strcmp(argv[1], "--memory") && strcmp(argv[1], "--gpu-memory"))) {
        fprintf(stderr, "Usage: %s --abi | --connect DEVICE | --heaps DEVICE | --memory DEVICE | --gpu-memory DEVICE\n", argv[0]);
        return 2;
    }
    fd = open(argv[2], O_RDWR | O_CLOEXEC);
    if (fd < 0) { perror("open"); return 1; }
    if (ioctl(fd, DRM_IOCTL_VERSION, &version)) { perror("DRM version"); goto out; }
    if (strcmp(name, "pvr") || version.version_major != 1 || version.version_minor != 9 ||
        version.version_patchlevel != PVRVERSION_BUILD) {
        fprintf(stderr, "Refusing incompatible driver: %s %d.%d@%d\n", name,
                version.version_major, version.version_minor, version.version_patchlevel);
        goto out;
    }
    if (call_bridge(fd, 1, PVRSRV_BRIDGE_SRVCORE_CONNECT, &input, sizeof(input), &output, sizeof(output))) {
        perror("CONNECT ioctl"); goto out;
    }
    printf("CONNECT error=%d kernel_arch=%u caps=%08x pvr=%08x rgx=%08x\n",
           output.eError, output.ui8KernelArch, output.ui32CapabilityFlags,
           output.ui32PVRBridges, output.ui32RGXBridges);
    if (output.eError != PVRSRV_OK) goto out;
    if (!strcmp(argv[1], "--heaps")) query_rc = dump_heaps(fd);
    if (!strcmp(argv[1], "--memory")) query_rc = memory_roundtrip(fd, 0);
    if (!strcmp(argv[1], "--gpu-memory")) query_rc = memory_roundtrip(fd, 1);
    if (call_bridge(fd, 1, PVRSRV_BRIDGE_SRVCORE_DISCONNECT, &disconnect, sizeof(disconnect),
                    &disconnected, sizeof(disconnected))) { perror("DISCONNECT ioctl"); goto out; }
    rc = disconnected.eError == PVRSRV_OK && !query_rc ? 0 : 1;
out:
    close(fd);
    return rc;
}
