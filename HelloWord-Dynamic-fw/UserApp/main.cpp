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

SysContext *g_sysCtx = nullptr;

void SysTask(TimerHandle_t t) {
  StorageTask(t);
  RGBTask();
  ButtonTask(t);
  timerSleepCallback(t);
}

template<class T>
void regApp() {
  auto lAppPtr = static_cast<T *> (pvPortMalloc(sizeof(T)));
  lAppPtr = new(lAppPtr) T();
  AppRegister(lAppPtr);
}

void helloWord() {
  CtrlInit();

  OLED_CLEAR_BUFFER();
  // logo区域
  g_sysCtx->Device.oled->SetDrawColor(1);
  g_sysCtx->Device.oled->DrawBox(6, 11, 20, 20);
  // 文字区域
  OLED_DEVICES()->DrawUTF8(9, 41, "屏");
  OLED_DEVICES()->DrawUTF8(9, 55, "幕");
  OLED_DEVICES()->DrawUTF8(9, 69, "自");
  OLED_DEVICES()->DrawUTF8(9, 83, "检");
  OLED_DEVICES()->DrawUTF8(9, 97, "中");
  g_sysCtx->Device.oled->SetDrawColor(0);
  g_sysCtx->Device.oled->DrawStr(6 + 3, 15, "HY");
  OLED_SEND_BUFFER();

  g_sysCtx->Device.eink->Init();
  //g_sysCtx->Device.eink->demo();
  ButtonPinInit();
  xTimerStart(xTimerCreate("SysTimer", pdMS_TO_TICKS(100), pdTRUE, nullptr, SysTask), 0);

  RGBInit();
}

void InitOs() {
  //初始化所有设备
  g_sysCtx = static_cast<SysContext *> (pvPortMalloc(sizeof(SysContext)));
  g_sysCtx = new(g_sysCtx) SysContext();

  g_sysCtx->Device.boardConfig = static_cast<BoardConfig_t *> (pvPortMalloc(sizeof(BoardConfig_t)));
  g_sysCtx->Device.boardConfig = new(g_sysCtx->Device.boardConfig) BoardConfig_t();

  g_sysCtx->Device.oled = static_cast<SSD1306 *> (pvPortMalloc(sizeof(SSD1306)));
  g_sysCtx->Device.oled = new(g_sysCtx->Device.oled) SSD1306(&hi2c1);
  g_sysCtx->Device.oled->Init();

  OLED_CLEAR_BUFFER();
  // logo区域
  g_sysCtx->Device.oled->SetDrawColor(1);
  g_sysCtx->Device.oled->DrawBox(6, 11, 20, 20);
  // 文字区域
  OLED_DEVICES()->DrawUTF8(9, 41, "系");
  OLED_DEVICES()->DrawUTF8(9, 55, "统");
  OLED_DEVICES()->DrawUTF8(9, 69, "启");
  OLED_DEVICES()->DrawUTF8(9, 83, "动");
  OLED_DEVICES()->DrawUTF8(9, 97, "中");
  g_sysCtx->Device.oled->SetDrawColor(0);
  g_sysCtx->Device.oled->DrawStr(6 + 3, 15, "HY");
  OLED_SEND_BUFFER();
  // 初始化存储
  InitStorage();

  g_sysCtx->Device.eink = static_cast<Eink290BW *> (pvPortMalloc(sizeof(Eink290BW)));
  g_sysCtx->Device.eink = new(g_sysCtx->Device.eink) Eink290BW();

  // APP在这注册
  regApp<AppDesktop>();
  regApp<AppWindows>();
  regApp<AppVolume>();
  regApp<AppUpDown>();
  regApp<AppLight>();
  regApp<AppLeftRight>();
  regApp<AppHID>();
  // 此处初始化所有APP
  for (auto &iter: g_sysCtx->Apps.AppsMap) {
    iter.second->Init();
  }

  //启动事件
  helloWord();
  AppChange(APPID_DESKTOP);
}

void test() {
  //InitStorage();
  g_sysCtx = static_cast<SysContext *> (pvPortMalloc(sizeof(SysContext)));
  g_sysCtx = new(g_sysCtx) SysContext();

  g_sysCtx->Device.boardConfig = static_cast<BoardConfig_t *> (pvPortMalloc(sizeof(BoardConfig_t)));
  g_sysCtx->Device.boardConfig = new(g_sysCtx->Device.boardConfig) BoardConfig_t();
  g_sysCtx->Device.eink = static_cast<Eink290BW *> (pvPortMalloc(sizeof(Eink290BW)));
  g_sysCtx->Device.eink = new(g_sysCtx->Device.eink) Eink290BW();
  g_sysCtx->Device.eink->Init();
  g_sysCtx->Device.eink->SendWhite();
  g_sysCtx->Device.eink->Update();
//  g_sysCtx->Device.oled = static_cast<SSD1306 *> (pvPortMalloc(sizeof(SSD1306)));
//  g_sysCtx->Device.oled = new(g_sysCtx->Device.oled) SSD1306(&hi2c1);
//  g_sysCtx->Device.oled->Init();
//
//  // logo区域
//  g_sysCtx->Device.oled->SetDrawColor(2);
//  g_sysCtx->Device.oled->DrawBox(6, 11, 20, 20);
//  // 文字区域
//  OLED_DEVICES()->DrawUTF8(9, 41, "调");
//  OLED_DEVICES()->DrawUTF8(9, 55, "试");
//  OLED_DEVICES()->DrawUTF8(9, 69, "模");
//  OLED_DEVICES()->DrawUTF8(9, 83, "式");
//  g_sysCtx->Device.oled->SetDrawColor(0);
//  g_sysCtx->Device.oled->DrawStr(6 + 3, 15, "HY");
//  OLED_SEND_BUFFER();
//
//  ButtonPinInit();
//  xTimerStart(xTimerCreate("SysTimer", pdMS_TO_TICKS(100), pdTRUE, nullptr, SysTask), 0);
//
//  RGBInit();
}

void Main(void) {
  InitOs();
  //test();
}

