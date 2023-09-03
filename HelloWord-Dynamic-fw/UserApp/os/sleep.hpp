//
// Created by 19254 on 2023/8/28.
//

#ifndef HELLOWORD_DYNAMIC_FW_SLEEP_HPP
#define HELLOWORD_DYNAMIC_FW_SLEEP_HPP
#include "os_define.hpp"
#include "timers.h"

// 初始化休眠定时器,不要多次调用
void SleepTimerInit();

// 注册休眠回调
int RegisterSleepCallBack(SysCallFunc func);
// 注册唤醒回调
int RegisterWakeUpCallBack(SysCallFunc func);

// 延迟系统休眠
void OSDelaySleep();
void timerSleepCallback(TimerHandle_t xTimer);

#endif //HELLOWORD_DYNAMIC_FW_SLEEP_HPP
