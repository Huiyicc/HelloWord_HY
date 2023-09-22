//
// Created by 19254 on 2023/8/28.
//

#ifndef HELLOWORD_DYNAMIC_FW_CTRL_H
#define HELLOWORD_DYNAMIC_FW_CTRL_H
#include "os_define.hpp"
#include "timers.h"

extern double lastAgent;
void CtrlInit();
bool RegisterKNobCallback(KnobCallback callback);
void taskCtrlLoop(TimerHandle_t);
void InitLastAgent();

#endif //HELLOWORD_DYNAMIC_FW_CTRL_H
