//
// Created by 19254 on 2023/8/28.
//
#include "ctrl.hpp"
#include "timers.h"
#include "sleep.hpp"


Timer timerCtrlLoop(&htim7, 5000);
osThreadId_t ctrlLoopTaskHandle;
TimerHandle_t g_timer_CtrlLoop = nullptr;


void ThreadCtrlLoop(void *) {
    for (;;) {
        // Suspended here until got Notification.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        g_sysCtx->Device.ctrl.knob.Tick();


    }
}

void OnTimerCallback() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Wake & invoke thread IMMEDIATELY.
    vTaskNotifyGiveFromISR(TaskHandle_t(ctrlLoopTaskHandle), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



//KnobStatus m_knobStatus;
//
//void OnTimerSampling(TimerHandle_t) {
//    m_knobStatus.LastPosition = m_knobStatus.Position;
//    m_knobStatus.Position = g_sysCtx->Device.ctrl->knob.GetPosition();
//    // 抖动容错 0.1
//    if (std::abs(m_knobStatus.Position-m_knobStatus.LastPosition)<0.1) {
//        return;
//    }
//    //唤醒
//    OSDelaySleep();
//    for (auto& iter:g_sysCtx->Device.ctrl->CallBacks) {
//        if (iter.second){
//            iter.second(&m_knobStatus);
//        }
//    }
//}

int RegisterKNobCallback(KnobCallback callback) {
    auto ritem = g_sysCtx->Device.ctrl.CallBacks.rbegin();
    if (ritem == g_sysCtx->Device.ctrl.CallBacks.rend()) {
        g_sysCtx->Device.ctrl.CallBacks[0] = callback;
        return 0;
    }
    g_sysCtx->Device.ctrl.CallBacks[ritem->first + 1] = callback;
    return ritem->first + 1;
}

osThreadId_t taskCtrlLoopHandle;

void taskCtrlLoop(void *) {
    KnobStatus knobStatus;
    for (;;) {
        osDelay(60);

        // 限幅滤波法
        // 后续优化吧,懒得写了
        auto l = g_sysCtx->Device.ctrl.knob.GetPosition();
        // 抖动容错
        if (std::abs(l - knobStatus.LastPositionRaw) < g_sysCtx->Device.ctrl.knob.filterateMax) {
            continue;
        }

        knobStatus.LastPositionRaw = knobStatus.PositionRaw;
        knobStatus.PositionRaw = l;
        knobStatus.LastPosition = knobStatus.Position;
        knobStatus.Position = l + g_sysCtx->Device.ctrl.knob.deviation;

        // 步数 6.3
        // 角度 = (步数 / 步数每圈) * 360°
        knobStatus.Angle = (knobStatus.Position / _2PI) * 360;

        // 唤醒事件
        OSDelaySleep();
        for (auto &iter: g_sysCtx->Device.ctrl.CallBacks) {
            if (iter.second) {
                iter.second(&knobStatus);
            }
        }
    }
}

void CtrlInit() {
    //MX_TIM7_Init();
    g_sysCtx->Device.ctrl.motor.AttachDriver(&g_sysCtx->Device.ctrl.driver);
    g_sysCtx->Device.ctrl.motor.AttachEncoder(&g_sysCtx->Device.ctrl.encoder);
    g_sysCtx->Device.ctrl.knob.Init(&g_sysCtx->Device.ctrl.motor);
    g_sysCtx->Device.ctrl.knob.SetEnable(true);
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_INERTIA);
    g_sysCtx->Device.ctrl.knob.Tick();

    const osThreadAttr_t controlLoopTask_attributes = {
            .name = "ControlLoopTask",
            .stack_size = 4096,
            .priority = (osPriority_t) osPriorityRealtime, // robot control thread is critical, should be the highest
    };
    ctrlLoopTaskHandle = osThreadNew(ThreadCtrlLoop, nullptr, &controlLoopTask_attributes);
    timerCtrlLoop.SetCallback(OnTimerCallback);
    timerCtrlLoop.Start();
    // 电机采样
    //g_timer_CtrlLoop = xTimerCreate("CtrlLoop", pdMS_TO_TICKS(2), pdTRUE, nullptr, OnTimerCallback);
    //xTimerStart(g_timer_CtrlLoop, 0);
    //xTaskCreate(taskCtrlLoop, "taskCtrlLoop", 100, NULL, osPriorityLow, NULL);
    const osThreadAttr_t taskCtrlLoop_attributes = {
            .name = "taskCtrlLoop",
            .stack_size = 2048,
            .priority = (osPriority_t) osPriorityNormal,
    };
    taskCtrlLoopHandle = osThreadNew(taskCtrlLoop, nullptr, &taskCtrlLoop_attributes);

}