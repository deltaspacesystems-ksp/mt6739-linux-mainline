/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include <string.h>
static char *(*reference_copy)(char *, const char *, size_t) = strncpy;
#define MFW_LINUX_COMPAT_TEST
#include "rogue-ddk19-port/include/mfw-linux-compat.h"
int main(void)
{
    const char *sources[] = { "", "abc", "0123456789012345678901234567890123456789" };
    char actual[64], expected[64];
    size_t i, size, n, length;
    unsigned int cases = 0;
    for (i = 0; i < sizeof(sources) / sizeof(sources[0]); i++) {
        length = strlen(sources[i]);
        for (size = 0; size <= 48; size++) {
            memset(actual, 0xa5, sizeof(actual)); memset(expected, 0xa5, sizeof(expected));
            assert(mfw_ddk_strncpy(actual + 4, sources[i], size) == actual + 4);
            reference_copy(expected + 4, sources[i], size);
            assert(!memcmp(actual, expected, sizeof(actual)));
            memset(actual, 0xa5, sizeof(actual)); memset(expected, 0xa5, sizeof(expected));
            assert(mfw_ddk_strlcpy(actual + 4, sources[i], size) == length);
            if (size) {
                n = length < size - 1 ? length : size - 1;
                memcpy(expected + 4, sources[i], n); expected[4 + n] = 0;
            }
            assert(!memcmp(actual, expected, sizeof(actual)));
            cases++;
        }
    }
    assert(mfw_ddk_strlcpy(NULL, "abc", 0) == 3);
    printf("PASS: %u bounded string cases, fixed-field padding/truncation, return values and guards\n", cases);
    return 0;
}
