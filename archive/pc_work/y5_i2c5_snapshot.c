/* SPDX-License-Identifier: GPL-2.0-only */
/* Read-only clock/controller snapshot; skips DATA_PORT and gated MMIO. */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void)
{
    int fd = open("/dev/mem", O_RDONLY | O_SYNC);
    if (fd < 0) return 2;
    volatile uint32_t *infra = mmap(0, 4096, PROT_READ, MAP_SHARED, fd, 0x10001000);
    if (infra == MAP_FAILED) return 2;
    uint32_t gate = infra[0xac/4];
    printf("INFRA_STA2=0x%08x I2C5_gate=%u I2C5A_gate=%u I2C5_IMM_gate=%u\n",
           gate, !!(gate & (1U<<18)), !!(gate & (1U<<19)), !!(gate & (1U<<20)));
    if (gate & (1U<<18)) { puts("I2C5 gated; controller not accessed"); return 0; }
    volatile uint16_t *bus = mmap(0, 4096, PROT_READ, MAP_SHARED, fd, 0x11016000);
    if (bus == MAP_FAILED) return 2;
    const unsigned offsets[] = {4, 8, 12, 16, 20, 24, 32, 40, 48, 108, 112};
    for (unsigned i=0; i < sizeof(offsets)/sizeof(offsets[0]); ++i)
        printf("I2C5[0x%02x]=0x%04x\n", offsets[i], bus[offsets[i]/2]);
    close(fd);
    return 0;
}
