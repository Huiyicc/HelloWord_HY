//
// Created by 19254 on 2023/9/11.
//
#include "common_inc.h"
#include "os_define.hpp"
#include "RGB.hpp"
#include "sleep.hpp"

struct rgbs {
    uint8_t n;
    uint8_t ststus;
};

RGB rgb(&hspi3);
rgbs rgbStatus[3] = {{0, 0}, {127, 0}, {255, 0}};

extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    rgb.isRgbTxBusy = false;
}

osThreadId_t RGBUpdateTaskHandle;

void rgbRanges(uint8_t index){
    if (rgbStatus[index].ststus==0) {
        if (rgbStatus[index].n == 255) {
            rgbStatus[index].ststus--;
        } else {
            rgbStatus[index].n++;
        }
    } else {
        if (rgbStatus[index].n == 0) {
            rgbStatus[index].ststus++;
        } else {
            rgbStatus[index].n--;
        }
    }
};

void ThreadRGBUpdate(void *argument) {
    auto dev = &g_sysCtx->Device.rgb;
    RGB::Color_t color = {0};
    if (GetSleepStatus()) {
        //rgb.SetLights(0);
        for (uint8_t i = 0; i < RGB::LED_NUMBER; i++)
            rgb.SetRgbBuffer(i, color, 0.2);
        rgb.SyncLights();
        return;
    }
    if (dev->effect == RGBEffect::Default_ || dev->effect == RGBEffect::Gradient) {
        rgbRanges(0);
        rgbRanges(1);
        rgbRanges(2);
    }
    color = RGB::Color_t{rgbStatus[0].n, rgbStatus[1].n, rgbStatus[2].n};
    for (uint8_t i = 0; i < RGB::LED_NUMBER; i++)
        rgb.SetRgbBuffer(i, color, 0.4);
    rgb.SyncLights();
//    for (;;) {
//
//
//    }
}

void RGBTask() {
    ThreadRGBUpdate(nullptr);
}

void RGBInit() {
    g_sysCtx->Device.rgb.rgb = &rgb;
//    const osThreadAttr_t peripheralTask_attributes = {
//            .name = "RGBUpdateTask",
//            .stack_size = 256,
//            .priority = (osPriority_t) osPriorityNormal,
//    };
//    RGBUpdateTaskHandle = osThreadNew(ThreadRGBUpdate, nullptr, &peripheralTask_attributes);
}