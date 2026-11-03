#include "gestures.h"
#include <assert.h>
#include <stdio.h>

int main(void){
    struct mfw_gestures g={0};
    /* Wayland IDs are arbitrary, not a zero-based finger count. */
    assert(mfw_gestures_down(&g,42,100,360,1435,720,1440));
    mfw_gestures_motion(&g,42,362,1300);
    assert(mfw_gestures_up(&g,42,250).action==ACTION_DESKTOP);
    assert(mfw_gestures_down(&g,77,100,360,1435,720,1440));
    mfw_gestures_motion(&g,77,360,1300);
    assert(mfw_gestures_up(&g,77,600).action==ACTION_OVERVIEW);
    /* A two-finger gesture must not turn into a tap when fingers lift in reverse order. */
    assert(mfw_gestures_down(&g,9,100,100,200,720,1440));
    assert(mfw_gestures_down(&g,51,101,150,205,720,1440));
    mfw_gestures_motion(&g,9,100,300);mfw_gestures_motion(&g,51,150,310);
    assert(mfw_gestures_up(&g,51,200).action==ACTION_NONE);
    struct mfw_gesture_result r=mfw_gestures_up(&g,9,210);
    assert(r.action==ACTION_LEAVE_FULLSCREEN&&r.fingers==2);
    for(int i=0;i<3;i++)assert(mfw_gestures_down(&g,10+i*7,100,100+i*20,600,720,1440));
    for(int i=0;i<3;i++)mfw_gestures_motion(&g,10+i*7,100+i*20,400);
    assert(mfw_gestures_up(&g,17,200).action==ACTION_NONE);
    assert(mfw_gestures_up(&g,10,201).action==ACTION_NONE);
    assert(mfw_gestures_up(&g,24,202).action==ACTION_OVERVIEW);
    /* Cancellation and duplicate ups must never create an action. */
    assert(mfw_gestures_down(&g,99,100,50,50,720,1440));mfw_gestures_cancel(&g);
    assert(mfw_gestures_up(&g,99,500).action==ACTION_NONE);
    assert(mfw_gestures_down(&g,3,UINT32_MAX-100,360,1435,720,1440));
    mfw_gestures_motion(&g,3,360,1300);
    assert(mfw_gestures_up(&g,3,50).action==ACTION_DESKTOP);
    puts("PASS: arbitrary contact IDs, multi-contact release ordering, cancellation and timestamp rollover");
    return 0;
}
