/* SPDX-License-Identifier: GPL-2.0-only */
/* Bounded, read-only evdev test: no grab, injection or power action. */
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <poll.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static long long milliseconds(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts)) return -1;
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int main(void)
{
    struct pollfd p = {.fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK),
                       .events = POLLIN};
    struct input_event events[32];
    long long start = milliseconds();
    setvbuf(stdout, NULL, _IONBF, 0);
    if (p.fd < 0 || start < 0) { perror("evdev/clock"); return 2; }
    puts("READY: event0, 120 seconds, no grab");
    while (milliseconds() - start < 120000) {
        int r = poll(&p, 1, 500);
        if (r < 0) { if (errno == EINTR) continue; perror("poll"); return 2; }
        if (!r) continue;
        if (p.revents & (POLLERR | POLLHUP | POLLNVAL)) return 2;
        ssize_t n = read(p.fd, events, sizeof(events));
        if (n < 0) { if (errno == EAGAIN || errno == EINTR) continue; return 2; }
        if (!n || n % sizeof(events[0])) return 2;
        for (unsigned i = 0; i < (unsigned)n / sizeof(events[0]); ++i) {
            const struct input_event *e = &events[i];
            const char *name = e->code == KEY_POWER ? "POWER" :
                e->code == KEY_VOLUMEUP ? "VOLUMEUP" :
                e->code == KEY_VOLUMEDOWN ? "VOLUMEDOWN" : "other";
            printf("t_ms=%lld type=%u code=%u value=%d %s\n",
                   milliseconds() - start, e->type, e->code, e->value,
                   e->type == EV_KEY ? name : "sync");
        }
    }
    close(p.fd);
    puts("DONE");
    return 0;
}
