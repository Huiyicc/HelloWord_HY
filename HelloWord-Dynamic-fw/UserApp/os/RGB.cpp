//
// Created by 19254 on 2023/9/11.
//
#include "common_inc.h"
#include "os_define.hpp"
#include "RGB.hpp"
#include "sleep.hpp"

RGB rgb(&hspi3);
uint8_t rgbStatus[3] = {0, 127, 255};

extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    rgb.isRgbTxBusy = false;
}


void RGBTask() {
    RGBUpdate();
}

osThreadId_t RGBUpdateTaskHandle;

void ThreadRGBUpdate(void *argument) {
    auto dev = &g_sysCtx->Device.rgb;
    for (;;) {
        RGB::Color_t color = {0};
        if (!GetSleepStatus()) {
            rgb.SetLights(0);
            rgb.SyncLights();
            return;
        }
        if (dev->effect == RGBEffect::Default_ || dev->effect == RGBEffect::Gradient) {
            rgbStatus[0] = rgbStatus[0] == 255 ? 0 : rgbStatus[0] + 1;
            rgbStatus[1] = rgbStatus[1] == 255 ? 0 : rgbStatus[1] + 1;
            rgbStatus[2] = rgbStatus[2] == 255 ? 0 : rgbStatus[2] + 1;
        }
        color = RGB::Color_t{rgbStatus[0], rgbStatus[1], rgbStatus[2]};
        for (uint8_t i = 0; i < RGB::LED_NUMBER; i++)
            rgb.SetRgbBuffer(i, color);
        rgb.SyncLights();
        delay(100);
    }
}

void RGBInit() {
    g_sysCtx->Device.rgb.rgb = &rgb;
//    const osThreadAttr_t peripheralTask_attributes = {
//            .name = "RGBUpdateTask",
//            .stack_size = 256,
//            .priority = (osPriority_t) osPriorityNormal,
//    };
    //RGBUpdateTaskHandle = osThreadNew(ThreadRGBUpdate, nullptr, &peripheralTask_attributes);
}