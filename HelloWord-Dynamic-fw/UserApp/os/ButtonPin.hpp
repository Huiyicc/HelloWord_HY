//
// Created by 19254 on 2023/8/2.
//
#ifndef HELLOWORD_DYNAMIC_FW_BUTTONPIN_H
#define HELLOWORD_DYNAMIC_FW_BUTTONPIN_H
#include "os_define.hpp"
extern ButtonPinCall ButtonPin;


void ButtonPinInit();
bool RegisterButtonPinCall(const char *tag, ButtonPinCall call);

#endif //HELLOWORD_DYNAMIC_FW_BUTTONPIN_H
