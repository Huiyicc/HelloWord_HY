//
// Created by 19254 on 2023/8/28.
//
#include <string>
#include "ctrl.hpp"
#include "timers.h"
#include "sleep.hpp"
#include "SDK/utils.hpp"


Timer timerCtrlLoop(&htim7, 5000);
osThreadId_t ctrlLoopTaskHandle;
//TimerHandle_t g_timer_CtrlLoop = nullptr;


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


bool RegisterKNobCallback(KnobCallback callback) {
    if (!callback) {
        return false;
    }
    g_sysCtx->Device.ctrl.CallBacks.PushBack(callback);
    return true;
}

osThreadId_t taskCtrlLoopHandle;
double lastPosition = 0;
int lastEncodePosition = 0;
KnobStatus *knobStatus = nullptr;

void taskCtrlLoop(void *) {
    for (;;) {
        osDelay(60);
        // 限幅滤波法
        // 后续优化吧,懒得写了
        auto l = g_sysCtx->Device.ctrl.knob.GetPosition();
//        OLED_CLEAR_BUFFER()
//        OLED_DEVICES()->SetDrawColor(1);
//        OLED_DEVICES()->DrawStr(0, 10, std::to_string(g_sysCtx->Device.ctrl.knob.GetVelocity()).c_str());
//        OLED_SEND_BUFFER();
        // 抖动容错
        if (std::abs(l - knobStatus->LastPositionRaw) < g_sysCtx->Device.ctrl.knob.filterateMax) {
            if (std::fabs(lastPosition - l) < g_sysCtx->Device.ctrl.knob.filterateMax) {
                continue;
            }
        }
        if (g_sysCtx->Device.ctrl.knob.GetMode() == KnobSimulator::Mode_t::MODE_ENCODER) {
            knobStatus->LastEncoderPosition = lastEncodePosition;
            knobStatus->EncoderPosition = g_sysCtx->Device.ctrl.knob.encoderPosition;
            lastEncodePosition = knobStatus->EncoderPosition;
        }
        lastPosition = l;
        knobStatus->LastPositionRaw = knobStatus->PositionRaw;
        knobStatus->PositionRaw = l;
        knobStatus->LastPosition = knobStatus->Position;
        knobStatus->Position = l + g_sysCtx->Device.ctrl.knob.deviation;

        // 步数 6.3
        // 角度 = (步数 / 步数每圈) * 360°
        knobStatus->Angle = (knobStatus->Position / _2PI) * 360;
        // 唤醒事件
        OSDelaySleep();
        knobStatus->Velocity = g_sysCtx->Device.ctrl.knob.GetVelocity();
        auto ptr = g_sysCtx->Device.ctrl.CallBacks.GetHeadPtr();
        while (ptr) {
            if (ptr->val) {
                ptr->val(knobStatus);
            }
            ptr = ptr->pNext;
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
    knobStatus = (KnobStatus *) pvPortMalloc(sizeof(KnobStatus));
    knobStatus = new(knobStatus) KnobStatus();
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
    //xTimerStart(xTimerCreate("CtrlLoop", pdMS_TO_TICKS(60), pdTRUE, nullptr, taskCtrlLoop),0);
    const osThreadAttr_t taskCtrlLoop_attributes = {
            .name = "taskCtrlLoop",
            .stack_size = 3072,
            .priority = (osPriority_t) osPriorityNormal,
    };
    taskCtrlLoopHandle = osThreadNew(taskCtrlLoop, nullptr, &taskCtrlLoop_attributes);

}