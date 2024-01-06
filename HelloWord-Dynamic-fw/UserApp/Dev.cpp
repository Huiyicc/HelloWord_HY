//
// Created by 19254 on 2024/1/5.
//

#include "Dev.hpp"
#include "common_inc.h"
#include "os/apps.hpp"
#include "os/app_desktop.hpp"
#include "os/app_volume.hpp"
#include "os/app_updown.hpp"
#include "os/app_light.hpp"
#include "os/app_windows.hpp"
#include "os/app_leftright.hpp"
#include "os/app_hid.hpp"
#include "os/ctrl.hpp"
#include "os/sleep.hpp"
#include "os/ButtonPin.hpp"
#include "os/RGB.hpp"
#include "SDK/utils.hpp"
#include "os/storage.hpp"

void dev_test() {
  //初始化所有设备
  g_sysCtx = static_cast<SysContext *> (pvPortMalloc(sizeof(SysContext)));
  g_sysCtx = new(g_sysCtx) SysContext();
  g_sysCtx->Device.boardConfig = static_cast<BoardConfig_t *> (pvPortMalloc(sizeof(BoardConfig_t)));
  g_sysCtx->Device.boardConfig = new(g_sysCtx->Device.boardConfig) BoardConfig_t();
  g_sysCtx->Device.oled = static_cast<SSD1306 *> (pvPortMalloc(sizeof(SSD1306)));
  g_sysCtx->Device.oled = new(g_sysCtx->Device.oled) SSD1306(&hi2c1);
  g_sysCtx->Device.oled->Init();
  // 初始化存储
  InitStorage();
  g_sysCtx->Device.eink = static_cast<Eink290BW *> (pvPortMalloc(sizeof(Eink290BW)));
  g_sysCtx->Device.eink = new(g_sysCtx->Device.eink) Eink290BW();
  g_sysCtx->Device.eink->Init();
  //g_sysCtx->Device.eink->demo();

}
