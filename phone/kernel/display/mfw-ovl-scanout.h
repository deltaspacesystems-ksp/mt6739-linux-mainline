/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_OVL_SCANOUT_H
#define MFW_OVL_SCANOUT_H
/* First reusable atomic-check/layout component for the MT6739 DRM port.
 * No MMIO, allocation or physical-address userspace ABI. Call ONLY with a
 * pinned GEM DMA object's DMA address and verified contiguous object size.
 * Plane commit/IRQ/fences and DSI initialization remain separate work.
 */
#ifdef __KERNEL__
#include <linux/errno.h>
#include <linux/types.h>
#else
#include <errno.h>
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif

#define MFW_XRGB8888 0x34325258U /* DRM_FORMAT_XRGB8888, XR24 */
#define MFW_WIDTH 720U
#define MFW_HEIGHT 1440U
#define MFW_PITCH 2880U

struct mfw_ovl_scanout {
 u32 address_offset;
 u32 pitch_offset;
 u32 size_offset;
 u32 address;
 u32 pitch;
 u32 pitch_mask;
 u32 size;
 u64 required_bytes;
};

static inline int mfw_ovl_prepare_scanout(unsigned int layer, u64 dma,
 u64 object_bytes, u32 width, u32 height, u32 pitch, u32 fourcc,
 struct mfw_ovl_scanout *out)
{
 u64 bytes;

 if (!out || layer > 3 || width != MFW_WIDTH || height != MFW_HEIGHT ||
     pitch != MFW_PITCH || fourcc != MFW_XRGB8888)
  return -EINVAL;
 /* Restrict the first port to page-aligned GEM allocations. This is a
  * deliberate driver restriction, NOT an inferred hardware requirement. */
 if (!dma || (dma & 4095ULL))
  return -EINVAL;
 bytes = (u64)pitch * height;
 if (object_bytes < bytes)
  return -EINVAL;
 /* OVL address register is32bit; the full scanout must stay in range. */
 if (dma > 0xffffffffULL || bytes - 1 > 0xffffffffULL - dma)
  return -ERANGE;
 *out = (struct mfw_ovl_scanout) {
  .address_offset = 0xf40 + layer * 0x20,
  .pitch_offset = 0x44 + layer * 0x20,
  .size_offset = 0x38 + layer * 0x20,
  .address = (u32)dma,
  .pitch = pitch,
  .pitch_mask = 0xffff, /* Preserve unrelated upper register fields. */
  .size = (height << 16) | width,
  .required_bytes = bytes,
 };
 return 0;
}
#endif
