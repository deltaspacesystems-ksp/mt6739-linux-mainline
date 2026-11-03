/*************************************************************************/ /*!
@File
@Title          Common bridge header for rgxkicksync
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Declares common defines and structures used by both the client
                and server side of the bridge for rgxkicksync
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/


/* Exact generated1.9 structs, dependency-free aliases for native standalone diagnostic. */
#include <stdint.h>
typedef void* IMG_HANDLE;typedef uint32_t IMG_UINT32;typedef char IMG_CHAR;typedef int32_t PVRSRV_FENCE;typedef int32_t PVRSRV_TIMELINE;typedef uint32_t PVRSRV_ERROR;
typedef struct PVRSRV_BRIDGE_IN_RGXCREATEKICKSYNCCONTEXT_TAG
{
	IMG_HANDLE hPrivData;
} __attribute__((packed)) PVRSRV_BRIDGE_IN_RGXCREATEKICKSYNCCONTEXT;
typedef struct PVRSRV_BRIDGE_OUT_RGXCREATEKICKSYNCCONTEXT_TAG
{
	IMG_HANDLE hKickSyncContext;
	PVRSRV_ERROR eError;
} __attribute__((packed)) PVRSRV_BRIDGE_OUT_RGXCREATEKICKSYNCCONTEXT;
typedef struct PVRSRV_BRIDGE_IN_RGXDESTROYKICKSYNCCONTEXT_TAG
{
	IMG_HANDLE hKickSyncContext;
} __attribute__((packed)) PVRSRV_BRIDGE_IN_RGXDESTROYKICKSYNCCONTEXT;
typedef struct PVRSRV_BRIDGE_OUT_RGXDESTROYKICKSYNCCONTEXT_TAG
{
	PVRSRV_ERROR eError;
} __attribute__((packed)) PVRSRV_BRIDGE_OUT_RGXDESTROYKICKSYNCCONTEXT;
typedef struct PVRSRV_BRIDGE_IN_RGXKICKSYNC_TAG
{
	IMG_HANDLE hKickSyncContext;
	IMG_UINT32 ui32ClientCacheOpSeqNum;
	IMG_UINT32 ui32ClientFenceCount;
	IMG_HANDLE * phFenceUFOSyncPrimBlock;
	IMG_UINT32 * pui32FenceSyncOffset;
	IMG_UINT32 * pui32FenceValue;
	IMG_UINT32 ui32ClientUpdateCount;
	IMG_HANDLE * phUpdateUFOSyncPrimBlock;
	IMG_UINT32 * pui32UpdateSyncOffset;
	IMG_UINT32 * pui32UpdateValue;
	IMG_UINT32 ui32ServerSyncCount;
	IMG_UINT32 * pui32ServerSyncFlags;
	IMG_HANDLE * phServerSync;
	PVRSRV_FENCE hCheckFenceFD;
	PVRSRV_TIMELINE hTimelineFenceFD;
	IMG_CHAR * puiUpdateFenceName;
	IMG_UINT32 ui32ExtJobRef;
} __attribute__((packed)) PVRSRV_BRIDGE_IN_RGXKICKSYNC;
typedef struct PVRSRV_BRIDGE_OUT_RGXKICKSYNC_TAG
{
	PVRSRV_FENCE hUpdateFenceFD;
	PVRSRV_ERROR eError;
} __attribute__((packed)) PVRSRV_BRIDGE_OUT_RGXKICKSYNC;
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXKICKSYNC)==108,"1.9 kick-sync wire");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_RGXKICKSYNC)==8,"1.9 kick-sync return");
