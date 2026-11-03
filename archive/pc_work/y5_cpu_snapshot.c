/* SPDX-License-Identifier: GPL-2.0-only */
/* Read-only snapshot. No PLL, divider, mux or voltage writes. */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mfw-cpu-rate.h"
int main(void)
{
    int fd = open("/dev/mem", O_RDONLY | O_SYNC);
    if (fd < 0) return 2;
    volatile uint32_t *ap = mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x1000c000);
    volatile uint32_t *ao = mmap(0,4096,PROT_READ,MAP_SHARED,fd,0x1001b000);
    close(fd);
    if (ap == MAP_FAILED || ao == MAP_FAILED) return 2;
    unsigned pll=ap[0x204/4], div=ao[0x2c/4], mux=ao[0];
    printf("ARMPLL_CON1=0x%08x CLKDIV=0x%08x MUX=0x%08x\n", pll, div, mux);
    printf("CPU nominal frequency Hz=%llu\n", mfw_cpu_rate(pll,div,mux,26000000));
    return 0;
}
