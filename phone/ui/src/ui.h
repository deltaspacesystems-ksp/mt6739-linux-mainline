#ifndef MFW_UI_H
#define MFW_UI_H
#include <cairo.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
enum mfw_icon { ICON_WIFI, ICON_CELLULAR, ICON_GEAR, ICON_CLOCK, ICON_CAMERA, ICON_SUN, ICON_TORCH };
enum mfw_page { PAGE_DESKTOP, PAGE_SWITCHER, PAGE_CONTROL, PAGE_SETTINGS };
struct mfw_window_info { unsigned id; char title[128],app_id[128]; bool active,minimized,maximized,fullscreen; };
struct mfw_model {
    enum mfw_page page;
    bool demo, charging, wifi;
    int capacity, voltage_uv, temp_deci_c;
    unsigned long cpu_hz;
    double scale;
    char clock[8];
    char battery_status[32];
    struct mfw_window_info windows[16];
    unsigned n_windows;
};
void mfw_icon(cairo_t *,enum mfw_icon,double,double,double,double);
void mfw_sample(struct mfw_model *);
void mfw_render(cairo_t *,int,int,const struct mfw_model *);
int mfw_wayland(struct mfw_model *,bool windowed,int limit_frames);
int mfw_desktop(struct mfw_model *,int limit_frames,bool smoke_actions);
#ifdef __cplusplus
}
#endif
#endif
