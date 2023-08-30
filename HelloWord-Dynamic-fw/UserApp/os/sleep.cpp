//
// Created by 19254 on 2023/8/28.
//
#include "sleep.hpp"
#include "timers.h"
#include "apps.hpp"

void OSWakeUp();

std::map<unsigned char, SysCallFunc> g_SysWakeUp = {};
std::map<unsigned char, SysCallFunc> g_SysSleep = {};
// 创建一个5秒的定时器，周期为5000毫秒，优先级为0
// 当两次定时,也就是10秒内没有收到任何按键事件时，触发休眠事件
TimerHandle_t g_timer_sleep;
bool g_sleep_status = false;

void timerSleepCallback(TimerHandle_t xTimer) {
    // 状态为false,无操作,下一次定时将会休眠
    if (!g_sleep_status) {
        g_sleep_status = true;
        return;
    }
    // 休眠事件
    for (auto &item: g_SysSleep) {
        if (item.second) {
            if (!item.second(g_sysCtx)) {
                return;
            }
        }
    }
    g_sysCtx->Device.oled->Clear();
    g_sysCtx->Device.oled->SetPowerSave(1);
    // 睡眠模式
    __WFI();
    xTimerStop(g_timer_sleep, 0);
}

// 延迟系统休眠
void OSDelaySleep() {
    // 如果已经休眠,则唤醒
    if (g_sleep_status) {
        __WFE();
        g_sysCtx->Device.oled->Clear();
        g_sysCtx->Device.oled->SetPowerSave(0);
        OSWakeUp();
        AppChange(g_sysCtx->Apps.Status);
        xTimerReset(g_timer_sleep, 0);
        g_sleep_status = false;
        return;
    }
    // 否则延迟休眠
    g_sleep_status = false;
}

void OSWakeUp() {
    // 重置状态
    for (auto &item: g_SysWakeUp) {
        if (item.second) {
            if (!item.second(g_sysCtx)) {
                return;
            }
        }
    }
}

int RegisterWakeUpCallBack(SysCallFunc func) {
    if (func == nullptr) {
        return 0;
    }
    auto lastItem = g_SysWakeUp.rbegin();
    if (lastItem != g_SysWakeUp.rend()) {
        g_SysWakeUp[lastItem->first + 1] = func;
        return lastItem->first + 1;
    } else {
        g_SysWakeUp[1] = func;
        return 1;
    }
};

int RegisterSleepCallBack(SysCallFunc func) {
    if (func == nullptr) {
        return 0;
    }
    auto lastItem = g_SysSleep.rbegin();
    if (lastItem != g_SysSleep.rend()) {
        g_SysSleep[lastItem->first + 1] = func;
        return lastItem->first + 1;
    } else {
        g_SysSleep[1] = func;
        return 1;
    }
}

void SleepTimerInit() {
    // 启动休眠定时器
    g_timer_sleep = xTimerCreate("SleepTimer", pdMS_TO_TICKS(5000),
                                 pdTRUE, nullptr, timerSleepCallback);
    xTimerStart(g_timer_sleep, 0);
}
