/* Wypisuje globalne obiekty Wayland udostepniane przez kompozytor (jakie protokoly mamy do dyspozycji). */
#include <stdio.h>
#include <string.h>
#include <wayland-client.h>

static void on_global(void *data, struct wl_registry *reg, uint32_t name, const char *iface, uint32_t ver)
{
    (void)data; (void)reg;
    printf("%3u  %-52s v%u\n", name, iface, ver);
}

static void on_remove(void *data, struct wl_registry *reg, uint32_t name) { (void)data; (void)reg; (void)name; }

static const struct wl_registry_listener listener = { on_global, on_remove };

int main(void)
{
    struct wl_display *d = wl_display_connect(NULL);
    if (!d) { fprintf(stderr, "brak polaczenia z Wayland (WAYLAND_DISPLAY?)\n"); return 1; }
    struct wl_registry *r = wl_display_get_registry(d);
    wl_registry_add_listener(r, &listener, NULL);
    wl_display_roundtrip(d);
    wl_display_disconnect(d);
    return 0;
}
