/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
static int fake_ioctl(int, unsigned long, ...);
static int fake_open(const char *, int, ...);
static int fake_close(int);
static void *fake_mmap(void *, size_t, int, int, int, off_t);
static int fake_munmap(void *, size_t);
#define main mfw_probe_main
#define ioctl fake_ioctl
#define open fake_open
#define close fake_close
#define mmap fake_mmap
#define munmap fake_munmap
#include "native_pvr_probe.c"
#undef main
#undef ioctl
#undef open
#undef close
#undef mmap
#undef munmap

enum scenario { NORMAL, BAD_VERSION, OPEN_ERROR, COUNT_ERROR, COUNT_TOO_LARGE, DETAIL_ERROR, IOCTL_ERROR, ALLOC_ERROR, MAP_ERROR, INITIAL_NONZERO, FREE_ERROR };
static enum scenario scenario;
static unsigned opens, closes, connects, disconnects, counts, details;
static unsigned allocations, releases, maps, unmaps;
static uint32_t mock_memory[1024];
static int vm_mode, failcmd, live_ctx, live_heap, live_reservation, live_mapping;
static int fake_open(const char *path, int flags, ...)
{
    assert(!strcmp(path, "/mock/gpu")); assert(flags == (O_RDWR | O_CLOEXEC));
    opens++;
    if (scenario == OPEN_ERROR) { errno = ENOENT; return -1; }
    return 9;
}
static int fake_close(int fd) { assert(fd == 9); closes++; return 0; }
static void *fake_mmap(void *address, size_t length, int prot, int flags, int fd, off_t offset)
{
    assert(!address && length == 4096 && prot == (PROT_READ | PROT_WRITE));
    assert(flags == MAP_SHARED && fd == 9 && offset == 0x234 * 4096);
    assert(allocations == 1); maps++;
    if (scenario == MAP_ERROR) { errno = ENOMEM; return MAP_FAILED; }
    return mock_memory;
}
static int fake_munmap(void *address, size_t length)
{ assert(address == mock_memory && length == 4096); unmaps++; return 0; }
static int fake_ioctl(int fd, unsigned long request, ...)
{
    va_list args;
    void *data;
    assert(fd == 9);
    va_start(args, request); data = va_arg(args, void *); va_end(args);
    if (request == DRM_IOCTL_VERSION) {
        struct drm_version *v = data;
        assert(v->name_len >= 4);
        memcpy(v->name, "pvr", 4);
        v->version_major = 1; v->version_minor = scenario == BAD_VERSION ? 17 : 9;
        v->version_patchlevel = PVRVERSION_BUILD;
        return 0;
    }
    assert(request == MFW_BRIDGE_IOCTL);
    struct mfw_bridge_cmd *cmd = data;
    void *in = (void *)(uintptr_t)cmd->input;
    void *out = (void *)(uintptr_t)cmd->output;
    if (cmd->bridge_id == 1 && cmd->function_id == PVRSRV_BRIDGE_SRVCORE_CONNECT) {
        PVRSRV_BRIDGE_IN_CONNECT *input = in;
        PVRSRV_BRIDGE_OUT_CONNECT *output = out;
        assert(cmd->input_size == 16 && cmd->output_size == 17);
        assert(input->ui32Flags == (1U << 5));
        assert(input->ui32ClientBuildOptions == RGX_BUILD_OPTIONS);
        assert(input->ui32ClientDDKVersion == PVRVERSION_PACK(1, 9));
        assert(input->ui32ClientDDKBuild == PVRVERSION_BUILD);
        connects++;
        output->ui8KernelArch = 64; output->ui32CapabilityFlags = 0;
        output->ui32PVRBridges = 1; output->ui32RGXBridges = 1;
        output->eError = PVRSRV_OK;
        return 0;
    }
    if (cmd->bridge_id == 1 && cmd->function_id == PVRSRV_BRIDGE_SRVCORE_DISCONNECT) {
        assert(cmd->input_size == 4 && cmd->output_size == 4);
        disconnects++; ((PVRSRV_BRIDGE_OUT_DISCONNECT *)out)->eError = PVRSRV_OK;
        return 0;
    }
    assert(cmd->bridge_id == 6);
    if (cmd->function_id>=15 && cmd->function_id<=22) {
     unsigned f=cmd->function_id;int failed=failcmd==(int)f;
     PVRSRV_ERROR err=failed?PVRSRV_ERROR_INVALID_PARAMS:PVRSRV_OK;
     switch(f){
     case 15:{PVRSRV_BRIDGE_OUT_DEVMEMINTCTXCREATE *o=out;assert(cmd->input_size==4&&cmd->output_size==24);o->eError=err;o->hDevMemServerContext=failed?NULL:(void*)0x300;if(!failed)live_ctx=1;break;}
     case 17:{PVRSRV_BRIDGE_IN_DEVMEMINTHEAPCREATE *i=in;PVRSRV_BRIDGE_OUT_DEVMEMINTHEAPCREATE *o=out;assert(live_ctx&&i->hDevmemCtx==(void*)0x300&&i->sHeapBaseAddr.uiAddr==0x10000000&&i->uiHeapLength==4096&&i->ui32Log2DataPageSize==12);assert(cmd->input_size==28&&cmd->output_size==12);o->eError=err;o->hDevmemHeapPtr=failed?NULL:(void*)0x301;if(!failed)live_heap=1;break;}
     case 21:{PVRSRV_BRIDGE_IN_DEVMEMINTRESERVERANGE *i=in;PVRSRV_BRIDGE_OUT_DEVMEMINTRESERVERANGE *o=out;assert(live_heap&&i->hDevmemServerHeap==(void*)0x301&&i->uiLength==4096);assert(cmd->input_size==24&&cmd->output_size==12);o->eError=err;o->hReservation=failed?NULL:(void*)0x302;if(!failed)live_reservation=1;break;}
     case 19:{PVRSRV_BRIDGE_IN_DEVMEMINTMAPPMR *i=in;PVRSRV_BRIDGE_OUT_DEVMEMINTMAPPMR *o=out;assert(live_reservation&&i->hReservation==(void*)0x302&&i->hPMR==(void*)0x234);assert(cmd->input_size==28&&cmd->output_size==12);o->eError=err;o->hMapping=failed?NULL:(void*)0x303;if(!failed)live_mapping=1;break;}
     case 20:assert(live_mapping);if(!failed)live_mapping=0;((PVRSRV_BRIDGE_OUT_DEVMEMINTUNMAPPMR*)out)->eError=err;break;
     case 22:assert(live_reservation&&!live_mapping);if(!failed)live_reservation=0;((PVRSRV_BRIDGE_OUT_DEVMEMINTUNRESERVERANGE*)out)->eError=err;break;
     case 18:assert(live_heap&&!live_reservation);if(!failed)live_heap=0;((PVRSRV_BRIDGE_OUT_DEVMEMINTHEAPDESTROY*)out)->eError=err;break;
     case 16:assert(live_ctx&&!live_heap);if(!failed)live_ctx=0;((PVRSRV_BRIDGE_OUT_DEVMEMINTCTXDESTROY*)out)->eError=err;break;
     }
     return 0;
    }
    if (cmd->function_id == 10) {
        PVRSRV_BRIDGE_IN_PHYSMEMNEWRAMBACKEDLOCKEDPMR *input = in;
        PVRSRV_BRIDGE_OUT_PHYSMEMNEWRAMBACKEDLOCKEDPMR *output = out;
        assert(cmd->input_size == 52 && cmd->output_size == 12);
        assert(input->uiSize == 4096 && input->uiChunkSize == 4096);
        assert(input->ui32NumPhysChunks == 1 && input->ui32NumVirtChunks == 1);
        assert(input->ui32Log2PageSize == 12 && *input->pui32MappingTable == 0);
        assert(input->uiFlags & PVRSRV_MEMALLOCFLAG_ZERO_ON_ALLOC);
        allocations++; memset(mock_memory, 0, sizeof(mock_memory));
        if (scenario == INITIAL_NONZERO) mock_memory[17] = 1;
        output->hPMRPtr = scenario == ALLOC_ERROR ? NULL : (void *)(uintptr_t)0x234;
        output->eError = scenario == ALLOC_ERROR ? PVRSRV_ERROR_OUT_OF_MEMORY : PVRSRV_OK;
        return 0;
    }
    if (cmd->function_id == 8) {
        assert(cmd->input_size == 8 && cmd->output_size == 4);
        assert(((PVRSRV_BRIDGE_IN_PMRUNREFUNLOCKPMR *)in)->hPMR == (void *)(uintptr_t)0x234);
        assert(!maps || scenario == MAP_ERROR || unmaps == 1);
        assert(!live_mapping);
        releases++;
        ((PVRSRV_BRIDGE_OUT_PMRUNREFUNLOCKPMR *)out)->eError = scenario == FREE_ERROR ? PVRSRV_ERROR_INVALID_PARAMS : PVRSRV_OK;
        return 0;
    }
    if (cmd->function_id == 28) {
        PVRSRV_BRIDGE_OUT_HEAPCFGHEAPCOUNT *output = out;
        assert(cmd->input_size == 4 && cmd->output_size == 8);
        assert(((PVRSRV_BRIDGE_IN_HEAPCFGHEAPCOUNT *)in)->ui32HeapConfigIndex == 0);
        counts++;
        if (scenario == IOCTL_ERROR) { errno = EIO; return -1; }
        output->eError = scenario == COUNT_ERROR ? PVRSRV_ERROR_INVALID_PARAMS : PVRSRV_OK;
        output->ui32NumHeaps = scenario == COUNT_TOO_LARGE ? 65 : 3;
        return 0;
    }
    assert(cmd->function_id == 30);
    PVRSRV_BRIDGE_IN_HEAPCFGHEAPDETAILS *input = in;
    PVRSRV_BRIDGE_OUT_HEAPCFGHEAPDETAILS *output = out;
    assert(cmd->input_size == 20 && cmd->output_size == 40);
    assert(input->ui32HeapConfigIndex == 0 && input->ui32HeapIndex < 3);
    assert(input->ui32HeapNameBufSz == 128);
    details++;
    memset(input->puiHeapNameOut, 'x', input->ui32HeapNameBufSz);
    if(vm_mode){memset(input->puiHeapNameOut,0,128);memcpy(input->puiHeapNameOut,"General",8);}
    /* No terminator: client must bound its own diagnostic string. */
    output->sDevVAddrBase.uiAddr = 0x10000000;
    output->uiHeapLength = 0x1000;
    output->ui32Log2DataPageSizeOut = 12;
    output->ui32Log2ImportAlignmentOut = 12;
    output->ui32Log2TilingStrideFactorOut = 0;
    output->eError = scenario == DETAIL_ERROR ? PVRSRV_ERROR_INVALID_PARAMS : PVRSRV_OK;
    return 0;
}
static int run(enum scenario mode, const char *action)
{
    vm_mode=!strcmp(action,"--gpu-memory");live_ctx=live_heap=live_reservation=live_mapping=0;
    scenario = mode; opens = closes = connects = disconnects = counts = details = 0;
    allocations = releases = maps = unmaps = 0;
    char *args[] = { "probe", (char *)action, "/mock/gpu", NULL };
    return mfw_probe_main(3, args);
}
int main(void)
{
    assert(run(NORMAL, "--connect") == 0);
    assert(opens == 1 && closes == 1 && connects == 1 && disconnects == 1 && !counts);
    assert(run(NORMAL, "--heaps") == 0);
    assert(connects == 1 && disconnects == 1 && counts == 1 && details == 3 && closes == 1);
    assert(run(BAD_VERSION, "--connect") == 1 && !connects && closes == 1);
    assert(run(OPEN_ERROR, "--connect") == 1 && !connects && !closes);
    assert(run(COUNT_ERROR, "--heaps") == 1 && disconnects == 1 && !details);
    assert(run(COUNT_TOO_LARGE, "--heaps") == 1 && disconnects == 1 && !details);
    assert(run(DETAIL_ERROR, "--heaps") == 1 && disconnects == 1 && details == 1);
    assert(run(IOCTL_ERROR, "--heaps") == 1 && disconnects == 1 && !details);
    assert(run(NORMAL, "--memory") == 0 && allocations == 1 && releases == 1 && maps == 1 && unmaps == 1);
    assert(run(ALLOC_ERROR, "--memory") == 1 && !releases && !maps && disconnects == 1);
    assert(run(MAP_ERROR, "--memory") == 1 && releases == 1 && !unmaps && disconnects == 1);
    assert(run(INITIAL_NONZERO, "--memory") == 1 && releases == 1 && unmaps == 1 && disconnects == 1);
    assert(run(FREE_ERROR, "--memory") == 1 && releases == 1 && unmaps == 1 && disconnects == 1);
    assert(run(NORMAL,"--gpu-memory")==0&&releases==1&&!live_ctx&&!live_heap&&!live_reservation&&!live_mapping);
    const int failures[]={15,17,21,19,20,22,18,16};
    for(unsigned i=0;i<sizeof(failures)/sizeof(failures[0]);i++){
     failcmd=failures[i];assert(run(NORMAL,"--gpu-memory")==1&&disconnects==1&&closes==1);
     if(failcmd==20||failcmd==22||failcmd==18||failcmd==16)assert(!releases);
     else assert(releases==1&&!live_ctx&&!live_heap&&!live_reservation&&!live_mapping);
    }
    failcmd=0;
    puts("PASS: genuine1.9 CONNECT/heap/PMR wire ABI, mapping/failure cleanup and DISCONNECT");
    return 0;
}
