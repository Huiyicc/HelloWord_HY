//
// Created by 19254 on 2023/8/2.
//
#ifndef HELLOWORD_DYNAMIC_FW_BUTTONPIN_H
#define HELLOWORD_DYNAMIC_FW_BUTTONPIN_H
#include "os_define.hpp"
#include "timers.h"
extern ButtonPinCall ButtonPin;

void ButtonTask(TimerHandle_t xTimer);
void ButtonPinInit();
bool RegisterButtonPinCall(ButtonPinCall call);

#endif //HELLOWORD_DYNAMIC_FW_BUTTONPIN_H
