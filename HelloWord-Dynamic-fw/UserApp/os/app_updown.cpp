//
// Created by 19254 on 2023/8/30.
//

#include "app_updown.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"
#include "SDK/usbproto.hpp"
#include "storage.hpp"

void appUpDownButtonPinCallback(enum ButtonPinCallType type) {
  if (g_sysCtx->Apps.Status != APPID_UPDOWN) {
    return;
  }
  switch (type) {
    case ButtonPinCallType::LeftButtonPressed:
      AppChange(APPID_DESKTOP);
      break;
    default:
      break;
  }
}

void appUpDownKNobCallback(bool move,KnobStatus *status) {
  if (g_sysCtx->Apps.Status != APPID_UPDOWN||!move) {
    return;
  }
  auto x = status->PositionRaw - status->LastPositionRaw;
  if (fabs(x) < 0.1) {
    return;
  }
  auto &cfg = GetSysConfig()->apps;
  if (status->Position > status->LastPosition) {
    HYSDK::USB::SendMouseUP(cfg.UpDown.AddedValue);
  } else {
    HYSDK::USB::SendMouseDOWN(cfg.UpDown.AddedValue);
  }
}

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppUpDown::Init() {
  RegisterButtonPinCall(appUpDownButtonPinCallback);
  RegisterKNobCallback(appUpDownKNobCallback);
};

// 进入事件
void AppUpDown::In() {
  auto &cfg = GetSysConfig()->apps;
  //g_sysCtx->Device.ctrl.knob.SetEncoderModePos(cfg.UpDown.EncodePos);
  g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t(cfg.UpDown.Mode),&cfg.UpDown);
  g_sysCtx->Device.ctrl.knob.UpdateConf(&cfg.UpDown);
  ReView();
};

// 刷新事件
void AppUpDown::ReView() {
  OLED_CLEAR_BUFFER();
  drawA();
  OLED_SEND_BUFFER();
};

// 绘制界面(无上位机版本)
void AppUpDown::drawA() {
  OLED_DEVICES()->SetDrawColor(1);
  OLED_DEVICES()->DrawUTF8(10, 10, "↑");
  OLED_DEVICES()->DrawUTF8(10, 24, "UP");
  OLED_DEVICES()->DrawUTF8(4, 58, "滚动");
  OLED_DEVICES()->DrawUTF8(10, 89, "DN");
  OLED_DEVICES()->DrawUTF8(10, 104, "↓");
}

// 退出事件
void AppUpDown::Out() {

};

