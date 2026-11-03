#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
typedef void* IMG_HANDLE;typedef uint32_t IMG_UINT32;typedef uint8_t IMG_BYTE;typedef char IMG_CHAR;typedef int32_t PVRSRV_FENCE,PVRSRV_TIMELINE;typedef uint32_t PVRSRV_ERROR;typedef struct {uint64_t uiAddr;} IMG_DEV_VIRTADDR;
#define PACKED __attribute__((packed))
typedef struct {uint64_t addr;} pvr_dev_addr_t;
typedef struct PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT_TAG
{
	IMG_UINT32 ui32Priority;
	IMG_UINT32 ui32FrameworkCmdize;
	IMG_BYTE * psFrameworkCmd;
	IMG_HANDLE hPrivData;
	IMG_DEV_VIRTADDR sResumeSignalAddr;
} __attribute__((packed)) PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT;
typedef struct PVRSRV_BRIDGE_IN_RGXKICKCDM_TAG
{
	IMG_HANDLE hComputeContext;
	IMG_UINT32 ui32ClientCacheOpSeqNum;
	IMG_UINT32 ui32ClientFenceCount;
	IMG_HANDLE * phClientFenceUFOSyncPrimBlock;
	IMG_UINT32 * pui32ClientFenceOffset;
	IMG_UINT32 * pui32ClientFenceValue;
	IMG_UINT32 ui32ClientUpdateCount;
	IMG_HANDLE * phClientUpdateUFOSyncPrimBlock;
	IMG_UINT32 * pui32ClientUpdateOffset;
	IMG_UINT32 * pui32ClientUpdateValue;
	IMG_UINT32 ui32ServerSyncCount;
	IMG_UINT32 * pui32ServerSyncFlags;
	IMG_HANDLE * phServerSyncs;
	PVRSRV_FENCE hCheckFenceFd;
	PVRSRV_TIMELINE hUpdateTimeline;
	IMG_CHAR * puiUpdateFenceName;
	IMG_UINT32 ui32CmdSize;
	IMG_BYTE * psDMCmd;
	IMG_UINT32 ui32PDumpFlags;
	IMG_UINT32 ui32ExtJobRef;
	IMG_DEV_VIRTADDR ssRobustnessResetReason;
} __attribute__((packed)) PVRSRV_BRIDGE_IN_RGXKICKCDM;
struct pvr_srv_rgx_create_compute_context_cmd {
   uint64_t robustness_address;
   void *priv_data;
   uint8_t *reset_framework_cmd;
   uint8_t *static_compute_context_state;
   uint32_t context_flags;
   uint32_t reset_framework_cmd_size;
   uint32_t max_deadline_ms;
   uint32_t packed_ccb_size;
   /* RGX_CONTEXT_PRIORITY_... flags. */
   uint32_t priority;
   uint32_t static_compute_context_state_size;
} PACKED;
struct pvr_srv_rgx_kick_cdm2_cmd {
   uint64_t max_deadline_us;
   void *compute_context;
   uint32_t *client_update_offset;
   uint32_t *client_update_value;
   uint32_t *sync_pmr_flags;
   uint8_t *cdm_cmd;
   char *update_fence_name;
   void **client_update_ufo_sync_prim_block;
   void **sync_pmrs;
   int32_t check_fence;
   int32_t update_timeline;
   uint32_t client_update_count;
   uint32_t cmd_size;
   uint32_t ext_job_ref;
   uint32_t num_work_groups;
   uint32_t num_work_items;
   uint32_t pdump_flags;
   uint32_t sync_pmr_count;
} PACKED;
_Static_assert(sizeof(void*)==8,"64-bit ABI");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT)==32,"old create");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXKICKCDM)==132,"old kick");
int main(void){printf("old_create=%zu old_kick=%zu modern_create=%zu modern_kick=%zu old_cmd_ptr_offset=%zu old_size_offset=%zu\n",sizeof(PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT),sizeof(PVRSRV_BRIDGE_IN_RGXKICKCDM),sizeof(struct pvr_srv_rgx_create_compute_context_cmd),sizeof(struct pvr_srv_rgx_kick_cdm2_cmd),offsetof(PVRSRV_BRIDGE_IN_RGXKICKCDM,psDMCmd),offsetof(PVRSRV_BRIDGE_IN_RGXKICKCDM,ui32CmdSize));}
