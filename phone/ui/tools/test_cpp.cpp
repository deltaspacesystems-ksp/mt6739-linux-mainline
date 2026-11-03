#include "ui.h"
#include "gestures.h"
#include <cassert>
#include <cstring>
#include <iostream>

int main() {
    mfw_model model{};
    model.demo=true;
    model.scale=1.0;
    mfw_sample(&model); // Calls the actual C implementation from a C++ consumer.
    assert(model.capacity==73 && std::strcmp(model.clock,"9:41")==0);
    mfw_gestures contacts{};
    assert(mfw_gestures_down(&contacts,81,100,100,100,720,1440));
    const auto result=mfw_gestures_up(&contacts,81,150);
    assert(result.action==ACTION_TAP && result.fingers==1);
    std::cout << "PASS: native C model/contact API linked and used from C++17\n";
}
