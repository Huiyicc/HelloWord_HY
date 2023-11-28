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
//TimerHandle_t g_timer_sleep;
// false:未休眠,true:已休眠
bool g_sleep_status = false;
int g_sleep_time = 0;

bool GetSleepStatus() {
    return g_sleep_status;
};

void timerSleepCallback(TimerHandle_t xTimer) {
    if (g_sleep_status) {
        return;
    }
    // 100 * 100
    // 叠加状态
    g_sleep_time++;
    // 如果大于x秒,则休眠
    if ((g_sleep_time*100) >= 10000) {
        // 休眠事件
        g_sleep_status = true;
        g_sleep_time = 0;
        for (auto &item: g_SysSleep) {
            if (item.second) {
                if (!item.second(g_sysCtx)) {
                    return;
                }
            }
        }
        //清理屏幕
        g_sysCtx->Device.oled->Clear();
        g_sysCtx->Device.oled->SetPowerSave(1);
        // 关闭电机
        g_sysCtx->Device.ctrl.knob.SetEnable(false);
        // 睡眠模式
        // __WFE();
        return;
    }
}

//extern char knob_reenable;

// 延迟系统休眠
void OSDelaySleep() {
    // 如果已经休眠,则唤醒
    if (g_sleep_status) {
        //__WFE();
        // __SEV();
        // 开启屏幕
        g_sysCtx->Device.oled->Clear();
        g_sysCtx->Device.oled->SetPowerSave(0);
        // 开启电机
        //knob_reenable = 1;
        g_sysCtx->Device.ctrl.knob.SetEnable(true);
        // 重置状态
        for (auto &item: g_SysWakeUp) {
            if (item.second) {
                if (!item.second(g_sysCtx)) {
                    return;
                }
            }
        }
        AppChange(g_sysCtx->Apps.Status);
        //xTimerReset(g_timer_sleep, 0);
        g_sleep_status = false;
        g_sleep_time = 0;
        return;
    }
    // 否则延迟休眠
    g_sleep_status = false;
    g_sleep_time = 0;
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
//    g_timer_sleep = xTimerCreate("SleepTimer", pdMS_TO_TICKS(5000),
//                                 pdTRUE, nullptr, timerSleepCallback);
//    xTimerStart(g_timer_sleep, 0);
}
