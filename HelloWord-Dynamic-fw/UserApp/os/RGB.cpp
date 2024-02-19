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
float rgbBrightness = 0.4f;
bool rgbRunStatus = false;

RGB rgb(&hspi3);
rgbs RgbStatus[4] = {
    {0,   0, 0, 0},
    {100, 0, 0, 0},
    {140, 0, 0, 0},
    {180, 0, 0, 0},
};

extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  rgb.isRgbTxBusy = false;
}

// 轮询渐变
void RGBEffectGradient(char index, RGBConfig *c, rgbs *s,  float maxBrightness) {
  s->RGBSeed++;
  if (s->RGBSeed >= 360) {
    s->RGBSeed = 0;
  }
  Color_t color = {0, 0, 0};
  HSV2RGB(s->RGBSeed, 1.0, 1.0, &s->R, &s->G, &s->B);  // HSV模型转成RGB三原色
  color.r = s->R;
  color.g = s->G;
  color.b = s->B;
  rgb.SetRgbBuffer(index, color, maxBrightness);
}

// 呼吸灯效
void RGBEffectBreathe(char index, RGBConfig *c, rgbs *s, float maxBrightness) {
  Color_t color = {0, 0, 0};
  rgbBrightness = (rgbRunStatus ? rgbBrightness + 0.005f : rgbBrightness - 0.005f);
  if (rgbBrightness >= maxBrightness) {
    rgbBrightness = c->Brightness;
    rgbRunStatus = false;
  } else if (rgbBrightness <= 0.0) {
    rgbBrightness = 0.0;
    rgbRunStatus = true;
  }
  color.r = c->R;
  color.g = c->G;
  color.b = c->B;
  rgb.SetRgbBuffer(index, color, rgbBrightness);
}

void ThreadRGBUpdate(void *argument) {
  for (char i = 0; i < 4; i++) {
    RGBConfig *rgbConfig = nullptr;
    rgbs *rgbStatus = &RgbStatus[i];
    switch (i) {
      case 0: {
        rgbConfig = &g_SysConfig.devices.rgb.N0;
      }
        break;
      case 1: {
        rgbConfig = &g_SysConfig.devices.rgb.N1;
      }
        break;
      case 2: {
        rgbConfig = &g_SysConfig.devices.rgb.N2;
      }
        break;
      case 3: {
        rgbConfig = &g_SysConfig.devices.rgb.N3;
      }
        break;
      default:
        continue;
    }
    Color_t color = {0, 0, 0};
    float maxBrightness = GetSleepStatus()? g_SysConfig.devices.rgb.SleepBrightness : rgbConfig->Brightness;
    if (GetSleepStatus()) {
      if (g_SysConfig.devices.rgb.SleepOff) {
        color.r = rgbConfig->R;
        color.g = rgbConfig->G;
        color.b = rgbConfig->B;
        rgb.SetRgbBuffer(i, color, 0.01f);
        continue;
      }
    }

    if (rgbConfig->Effect == RGBEffect::Default_ ||
        rgbConfig->Effect == RGBEffect::Gradient) {
      // 轮询灯效
      RGBEffectGradient(i, rgbConfig, rgbStatus, maxBrightness);
      continue;
    } else if (rgbConfig->Effect == RGBEffect::Breathe) {
      // 呼吸灯效
      RGBEffectBreathe(i, rgbConfig, rgbStatus, maxBrightness);
      continue;
    } else if (rgbConfig->Effect == RGBEffect::Custom) {
      // 自定义灯效
      color.r = rgbConfig->R;
      color.g = rgbConfig->G;
      color.b = rgbConfig->B;
      rgb.SetRgbBuffer(i, color, maxBrightness);
      continue;
    }

  }
  rgb.SyncLights();

}

void RGBTask() {
  ThreadRGBUpdate(nullptr);
}

void RGBInit() {
  g_sysCtx->Device.rgb.rgb = &rgb;
}