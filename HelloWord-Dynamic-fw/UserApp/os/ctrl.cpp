//
// Created by 19254 on 2023/8/28.
//
#include "ctrl.hpp"
#include "SDK/utils.hpp"
#include "sleep.hpp"
#include "timers.h"
#include <string>

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
  //g_sysCtx->Device.ctrl.knob.Tick();
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
double lastPositionCache = 0;
int lastEncodePosition = 0;
KnobStatus *knobStatus = nullptr;
double lastAgent = 0;
char ctrSleep = 0;
//char knob_reenable = 0;

int ctrlKnobSleepWait(SysContext *ctx) {
  ctrSleep = 2;
  lastPositionCache = lastPosition;
  lastPosition = ctx->Device.ctrl.knob.GetPosition();
  return true;
}

int ctrlKnobSleepWakeUp(SysContext *ctx) {
  ctrSleep = 0;
  lastPosition = lastPositionCache;
  lastPositionCache = 0;
  return true;
}

void taskCtrlLoop(void *) {
  float filterateMax = 0.03;
  for (;;) {
    osDelay(50);
    if (!g_sysCtx->Device.ctrl.Action) {
      // 校准失败需要重新上电
      return;
    }

    auto l = g_sysCtx->Device.ctrl.knob.GetPosition();
    if (ctrSleep > 1) {
      if (ctrSleep < 20) {
        ctrSleep++;
        lastPosition = l;
        continue;
      } else {
        ctrSleep = 1;
      }
    }
    // 限幅滤波法
    // 后续优化吧,懒得写了
    // 抖动容错
    if (std::abs(l - knobStatus->LastPositionRaw) < filterateMax) {
      if (std::fabs(lastPosition - l) < filterateMax) {
        continue;
      }
    }
    // 唤醒事件
    if (GetSleepStatus()) {
      if (std::fabs(lastPosition - l) > 0.1){
        OSDelaySleep();
      } else {
        continue;
      }
    }
    if (g_sysCtx->Device.ctrl.knob.GetMode() == KnobSimulator::Mode_t::MODE_ENCODER || g_sysCtx->Device.ctrl.knob.GetMode() == KnobSimulator::Mode_t::MODE_JINLUNENCODER || g_sysCtx->Device.ctrl.knob.GetMode() == KnobSimulator::Mode_t::MODE_INTELLIGENT) {
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
    knobStatus->Velocity = g_sysCtx->Device.ctrl.knob.GetVelocity();
    auto ptr = g_sysCtx->Device.ctrl.CallBacks.GetHeadPtr();
    while (ptr) {
      if (ptr->val) {
        ptr->val(true, knobStatus);
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
  g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_INERTIA, nullptr);

  if (!g_sysCtx->Device.ctrl.Action) {
    OLED_CLEAR_BUFFER();
    // logo区域
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(font_default);
    OLED_DEVICES()->DrawBox(6, 11, 20, 20);
    OLED_DEVICES()->DrawUTF8(9, 41, "校");
    OLED_DEVICES()->DrawUTF8(9, 55, "准");
    OLED_DEVICES()->DrawUTF8(9, 69, "失");
    OLED_DEVICES()->DrawUTF8(9, 83, "败");
    OLED_DEVICES()->SetDrawColor(0);
    OLED_DEVICES()->DrawStr(6 + 3, 15, "HY");
    OLED_SEND_BUFFER();
    exit(1);
#ifdef _D_DEBUG_
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_DAMPED, nullptr);
    g_sysCtx->Device.ctrl.motor.target = g_sysCtx->Device.ctrl.knob.deviation;
    while (g_sysCtx->Device.ctrl.knob.GetPosition() != g_sysCtx->Device.ctrl.knob.deviation) {
      g_sysCtx->Device.ctrl.knob.Tick();
    }
    NVIC_SystemReset();
#endif
  }


  g_sysCtx->Device.ctrl.knob.Tick();
  knobStatus = (KnobStatus *) pvPortMalloc(sizeof(KnobStatus));
  knobStatus = new (knobStatus) KnobStatus();
  const osThreadAttr_t controlLoopTask_attributes = {
      .name = "ControlLoopTask",
      .stack_size = 4096,
      .priority = (osPriority_t) osPriorityRealtime,// robot control thread is critical, should be the highest
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
  RegisterSleepCallBack(ctrlKnobSleepWait);
  RegisterWakeUpCallBack(ctrlKnobSleepWakeUp);
}

void InitLastAgent() {
  lastAgent = (g_sysCtx->Device.ctrl.knob.GetPosition() / _2PI) * 360;
  ;
}
