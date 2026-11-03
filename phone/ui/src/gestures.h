#ifndef MFW_GESTURES_H
#define MFW_GESTURES_H
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
enum mfw_action { ACTION_NONE, ACTION_TAP, ACTION_DESKTOP, ACTION_OVERVIEW,
                  ACTION_CONTROL, ACTION_LEAVE_FULLSCREEN };
struct mfw_contact { int32_t id; bool used,down; double sx,sy,x,y; };
struct mfw_gestures {
    struct mfw_contact contacts[10];
    unsigned active,count,peak;
    uint32_t started;
    double width,height;
};
struct mfw_gesture_result { enum mfw_action action; double x,y; unsigned fingers; };
void mfw_gestures_cancel(struct mfw_gestures *);
bool mfw_gestures_down(struct mfw_gestures *,int32_t,uint32_t,double,double,double,double);
void mfw_gestures_motion(struct mfw_gestures *,int32_t,double,double);
struct mfw_gesture_result mfw_gestures_up(struct mfw_gestures *,int32_t,uint32_t);
#ifdef __cplusplus
}
#endif
#endif
