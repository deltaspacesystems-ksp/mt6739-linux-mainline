/* SPDX-License-Identifier: GPL-2.0-only */
/* Fault injection against the exact production fill helper; no GPU required. */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MFW_DMABUF_FILL_TEST
#define PAGE_SIZE 4096
#define GFP_KERNEL 0
#define DMA_TO_DEVICE 1
#define min_t(type, a, b) ((type)(a) < (type)(b) ? (type)(a) : (type)(b))
struct dma_buf { size_t size; unsigned char *data; };
struct iosys_map { void *vaddr; bool is_iomem; };
static int alloc_fail, begin_error, map_error, end_error, null_map, io_map;
static int allocations, frees, begins, ends, maps, unmaps, writes;
static void *kmalloc(size_t size, int flags)
{
    (void)flags;
    if (alloc_fail) return NULL;
    allocations++;
    return malloc(size);
}
static void kfree(void *p) { if (p) frees++; free(p); }
static int dma_buf_begin_cpu_access(struct dma_buf *buf, int dir)
{ (void)buf; assert(dir == DMA_TO_DEVICE); begins++; return begin_error; }
static int dma_buf_end_cpu_access(struct dma_buf *buf, int dir)
{ (void)buf; assert(dir == DMA_TO_DEVICE); ends++; return end_error; }
static int dma_buf_vmap_unlocked(struct dma_buf *buf, struct iosys_map *map)
{
    maps++;
    assert(begins == 1 && !begin_error && !ends);
    if (map_error) return map_error;
    map->vaddr = null_map ? NULL : buf->data;
    map->is_iomem = io_map;
    return 0;
}
static void dma_buf_vunmap_unlocked(struct dma_buf *buf, struct iosys_map *map)
{ (void)buf; (void)map; assert(maps == 1 && !map_error && !ends); unmaps++; }
static bool iosys_map_is_null(struct iosys_map *map) { return !map->vaddr; }
static void iosys_map_memset(struct iosys_map *map, size_t offset, int value, size_t size)
{ assert(!unmaps && !ends); writes++; memset((char *)map->vaddr + offset, value, size); }
static void iosys_map_memcpy_to(struct iosys_map *map, size_t offset, const void *src, size_t size)
{ assert(!unmaps && !ends); writes++; memcpy((char *)map->vaddr + offset, src, size); }
#include "rogue-ddk19-port/services/server/env/linux/mfw-dmabuf-fill.h"

static void reset(void)
{
    alloc_fail = begin_error = map_error = end_error = null_map = io_map = 0;
    allocations = frees = begins = ends = maps = unmaps = writes = 0;
}
int main(void)
{
    unsigned char storage[PAGE_SIZE + 17 + 32];
    const unsigned char pattern[] = "<DEAD-BEEF>";
    struct dma_buf buf = { PAGE_SIZE + 17, storage + 16 };
    size_t i;
    int io;
    for (io = 0; io <= 1; io++) {
        reset(); io_map = io; memset(storage, 0xa5, sizeof(storage));
        assert(mfw_fill_dmabuf(&buf, pattern, 11) == 0);
        for (i = 0; i < buf.size; i++) assert(buf.data[i] == pattern[(i % PAGE_SIZE) % 11]);
        for (i = 0; i < 16; i++) assert(storage[i] == 0xa5 && storage[sizeof(storage)-1-i] == 0xa5);
        assert(allocations == frees && unmaps == 1 && ends == 1 && writes == 2);
        reset(); io_map = io;
        assert(mfw_fill_dmabuf(&buf, NULL, 0) == 0);
        for (i = 0; i < buf.size; i++) assert(buf.data[i] == 0);
        assert(!allocations && unmaps == 1 && ends == 1);
    }
    reset(); assert(mfw_fill_dmabuf(&buf, pattern, 0) == -EINVAL); assert(!begins);
    reset(); alloc_fail = 1;
    assert(mfw_fill_dmabuf(&buf, pattern, 11) == -ENOMEM); assert(!begins);
    reset(); begin_error = -EBUSY;
    assert(mfw_fill_dmabuf(&buf, pattern, 11) == -EBUSY);
    assert(!maps && !ends && allocations == frees);
    reset(); map_error = -ENOMEM;
    assert(mfw_fill_dmabuf(&buf, pattern, 11) == -ENOMEM);
    assert(ends == 1 && !unmaps && !writes && allocations == frees);
    reset(); null_map = 1;
    assert(mfw_fill_dmabuf(&buf, pattern, 11) == -ENOMEM);
    assert(ends == 1 && unmaps == 1 && !writes && allocations == frees);
    reset(); end_error = -EIO;
    assert(mfw_fill_dmabuf(&buf, pattern, 11) == -EIO);
    assert(ends == 1 && unmaps == 1 && allocations == frees);
    reset(); map_error = -ENOMEM; end_error = -EIO;
    assert(mfw_fill_dmabuf(&buf, pattern, 11) == -ENOMEM);
    assert(ends == 1 && !unmaps && allocations == frees);
    puts("PASS: RAM/IOMEM dispatch, page poison, partial tail, guard bytes, zero-fill, allocation/begin/map/null/end failures and cleanup");
    return 0;
}
