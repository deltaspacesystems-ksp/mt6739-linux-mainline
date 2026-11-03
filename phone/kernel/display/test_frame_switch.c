/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-frame-switch.h"
int main(void){struct mfw_frame_switch s={0};
 assert(mfw_switch_request(&s,0)==-EINVAL);assert(mfw_switch_request(&s,1)==-EINVAL);
 for(unsigned i=0;i<100;i++){
  assert(!mfw_switch_request(&s,0x60000000));assert(mfw_switch_request(&s,0x61000000)==-EBUSY);
  assert(mfw_switch_eof(&s)==MFW_SWITCH_APPLY&&s.target==0x60000000);
  assert(mfw_switch_request(&s,0x61000000)==-EBUSY);
  assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);assert(mfw_switch_eof(&s)==MFW_SWITCH_COMPLETE);
  assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);
 }
 assert(!mfw_switch_request(&s,0x60000000));mfw_switch_cancel(&s);assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);
 assert(!mfw_switch_request(&s,0x60000000));assert(mfw_switch_eof(&s)==MFW_SWITCH_APPLY);
 mfw_switch_cancel(&s);assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);assert(!mfw_switch_request(&s,0x61000000));
 puts("PASS: serialized frame switch, conservative retirement delay, idle EOF, cancellation and invalid targets");return 0;}
