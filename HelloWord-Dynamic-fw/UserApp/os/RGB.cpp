//
// Created by 19254 on 2023/9/11.
//
#include "common_inc.h"
#include "os_define.hpp"
#include "RGB.hpp"
#include "sleep.hpp"
#include <cmath>

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

/*  HSV2RGB:
 *  Converts from HSV colorspace to RGB values.
 *  HSV2RGB(iHSV_H / 3.0, 1, 1, &r, &g, &b);
 */
void HSV2RGB(float h, float s, float v, int *r, int *g, int *b) {
  float f, x, y, z;
  int i;
  v *= 255.0;
  if (s == 0.0) {
    *r = *g = *b = (int) v;
  } else {
    while (h < 0)
      h += 360;
    h = fmod(h, 360) / 60.0;
    i = (int) h;
    f = h - i;
    x = v * (1.0 - s);
    y = v * (1.0 - (s * f));
    z = v * (1.0 - (s * (1.0 - f)));
    switch (i) {
      case 0:
        *r = v;
        *g = z;
        *b = x;
        break;
      case 1:
        *r = y;
        *g = v;
        *b = x;
        break;
      case 2:
        *r = x;
        *g = v;
        *b = z;
        break;
      case 3:
        *r = x;
        *g = y;
        *b = v;
        break;
      case 4:
        *r = z;
        *g = x;
        *b = v;
        break;
      case 5:
        *r = v;
        *g = x;
        *b = y;
        break;
    }
  }
}

struct rgbs {
    uint16_t RGBSeed;
    int R = 0;
    int G = 0;
    int B = 0;
};
double rgbBrightness = 0.4f;
bool rgbRunStatus = false;

RGB rgb(&hspi3);
rgbs rgbStatus[4] = {
    {0,   0, 0, 0},
    {127, 0, 0, 0},
    {127, 0, 0, 0},
    {127, 0, 0, 0},
};

extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  rgb.isRgbTxBusy = false;
}

void ThreadRGBUpdate(void *argument) {
  for (char i = 0; i < 4; i++) {
    RGBConfig *rgbConfig = nullptr;
    switch (i) {
      case 0:
        rgbConfig = &g_SysConfig.devices.rgb.N0;
        break;
      case 1:
        rgbConfig = &g_SysConfig.devices.rgb.N1;
        break;
      case 2:
        rgbConfig = &g_SysConfig.devices.rgb.N2;
        break;
      case 3:
        rgbConfig = &g_SysConfig.devices.rgb.N3;
        break;
      default:
        continue;
    }
    RGB::Color_t color = {0};
    if (GetSleepStatus()) {
      if (g_SysConfig.devices.rgb.SleepOff) {
        for (uint8_t i = 0; i < RGB::LED_NUMBER; i++) {
          color.r = rgbConfig->R;
          color.g = rgbConfig->G;
          color.b = rgbConfig->B;
          rgb.SetRgbBuffer(i, color, g_SysConfig.devices.rgb.SleepBrightness);
        }
        rgb.SyncLights();
        return;
      }
    }

    if (g_SysConfig.devices.rgb.Effect == RGBEffect::Default_ ||
        g_SysConfig.devices.rgb.Effect == RGBEffect::Gradient) {
      // 轮询灯效
      rgbStatus[i].RGBSeed++;
      if (rgbStatus[i].RGBSeed >= 360) {
        rgbStatus[i].RGBSeed = 0;
      }
      HSV2RGB(rgbStatus[i].RGBSeed, 1.0, 1.0, &rgbStatus[i].R, &rgbStatus[i].G, &rgbStatus[i].B);  // HSV模型转成RGB三原色
      color.r = rgbStatus[i].R;
      color.g = rgbStatus[i].G;
      color.b = rgbStatus[i].B;
      rgb.SetRgbBuffer(i, color, 0.4);
    } else if (g_SysConfig.devices.rgb.Effect == RGBEffect::Breathe) {
      // 呼吸灯效
      rgbBrightness = (rgbRunStatus ? rgbBrightness + 0.005 : rgbBrightness - 0.005);
      if (rgbBrightness >= 1.0) {
        rgbBrightness = 1.0;
        rgbRunStatus = false;
      } else if (rgbBrightness <= 0.0) {
        rgbBrightness = 0.0;
        rgbRunStatus = true;
      }
      color.r = rgbConfig->R;
      color.g = rgbConfig->G;
      color.b = rgbConfig->B;
      rgb.SetRgbBuffer(i, color, rgbBrightness);
    }
    rgb.SyncLights();
  }

}

void RGBTask() {
  ThreadRGBUpdate(nullptr);
}

void RGBInit() {
  g_sysCtx->Device.rgb.rgb = &rgb;
}