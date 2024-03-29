//
// Created by 19254 on 2023/8/27.
//
#include "app_windows.hpp"
#include "ButtonPin.hpp"
#include "SDK/usbproto.hpp"
#include "SDK/utils.hpp"
#include "ctrl.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"

void appWindowsButtonPinCallback(enum ButtonPinCallType type) {
  if (g_sysCtx->Apps.Status != APPID_WINDOW) {
    return;
  }
  switch (type) {
    case ButtonPinCallType::LeftButtonPressed:
      AppChange(APPID_DESKTOP);
      break;
    case ButtonPinCallType::RightButtonPressed: {
      auto _this = ((AppWindows *) (g_sysCtx->Apps.AppsMap[APPID_WINDOW]));
      _this->SwitchMode();
      _this->ReView();
    } break;
    default:
      break;
  }
}

void appWindowsKNobCallback(bool move, KnobStatus *status) {
  if (g_sysCtx->Apps.Status != APPID_WINDOW || !move) {
    return;
  }
  auto _this = ((AppWindows *) (g_sysCtx->Apps.AppsMap[APPID_WINDOW]));
  uint8_t keyBoardData[9] = {2, 0, 0, 0, 0, 0, 0, 0};
  uint8_t keyBoardDataEmpty[9] = {2, 0, 0, 0, 0, 0, 0, 0};
  auto x = status->Position;
  auto p = g_sysCtx->Device.ctrl.knob.paddle;
  p = p - x;
  if (fabs(p) < 0.8           // 位置小于防抖区间
      && _this->IsSend == 0   // 从未发送按键信息
      && _this->IsPrepare != 0// 上次位置超过上报准备区
  ) {
    // 上报按键信息
    switch (_this->IsPrepare) {
      case 1:
        // 上推
        // LWIN+TAB
//        keyBoardData[1] = ((1 << ((0x84 & 0x0f) - 1)));
//        keyBoardData[3] = HYSDK::USB::KeyCode_t::TAB;
        if (_this->GetMode() == 'W') {
          // LALT+LSHIFT+TAB
          keyBoardData[1] = 0x06;
          keyBoardData[2] = 0x00;
          keyBoardData[3] = HYSDK::USB::KeyCode_t::TAB;

        } else {
          // LCommand+W
          keyBoardData[1] = 0x08;
          keyBoardData[2] = 0x00;
          keyBoardData[3] = HYSDK::USB::KeyCode_t::W;
        }

        HYSDK::USB::USBDHIDSendReport(&hUsbDeviceFS, (uint8_t *) &keyBoardData, 9, 2);
        HYSDK::USB::USBDHIDSendReport(&hUsbDeviceFS, (uint8_t *) &keyBoardDataEmpty, 9, 2);

        break;
      case 2:
        // 下拉
//        // LWIN+D
//        keyBoardData[1] = ((1 << ((0x84 & 0x0f) - 1)));
//        keyBoardData[3] = HYSDK::USB::KeyCode_t::D;
        if (_this->GetMode() == 'W') {
          // LWIN+TAB
          keyBoardData[1] = 0x08;
          keyBoardData[2] = 0x00;
          keyBoardData[3] = HYSDK::USB::KeyCode_t::TAB;
        } else {
          // LCommand+TAB
          keyBoardData[1] = 0x08;
          keyBoardData[2] = 0x00;
          keyBoardData[3] = HYSDK::USB::KeyCode_t::TAB;
        }
        HYSDK::USB::USBDHIDSendReport(&hUsbDeviceFS, (uint8_t *) &keyBoardData, 9, 2);
        HYSDK::USB::USBDHIDSendReport(&hUsbDeviceFS, (uint8_t *) &keyBoardDataEmpty, 9, 2);

        break;
      default:
        break;
    }
    _this->IsSend == 1;
    _this->IsPrepare = 0;
    return;
  }
  if (_this->IsPrepare != 0 && _this->IsSend == 0) {
    return;
  }
  if (p > 0.7) {
    // 开启下拉
    _this->IsPrepare = 2;
    _this->IsSend = 0;
    return;
  } else if (p < -0.7) {
    // 开启上推
    _this->IsPrepare = 1;
    _this->IsSend = 0;
    return;
  }
}

//初始化事件
void AppWindows::Init() {
  RegisterButtonPinCall(appWindowsButtonPinCallback);
  RegisterKNobCallback(appWindowsKNobCallback);
}

// 进入事件
void AppWindows::In() {
  g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_PADDLE, nullptr);
  ReView();
}

// 刷新事件
void AppWindows::ReView() {
  OLED_CLEAR_BUFFER();
  if (m_model == 'W') {
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(font_icon);
    OLED_DEVICES()->DrawUTF8(4, 52, "W");
    OLED_DEVICES()->SetFont(font_default);
    OLED_DEVICES()->DrawUTF8(10, 10, "↑");
    OLED_DEVICES()->DrawUTF8(4, 24, "切换");
    OLED_DEVICES()->DrawUTF8(4, 89, "视图");
    OLED_DEVICES()->DrawUTF8(10, 104, "↓");
  } else if (m_model == 'M') {
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(font_icon);
    OLED_DEVICES()->DrawUTF8(4, 52, "M");
    OLED_DEVICES()->SetFont(font_default);
    OLED_DEVICES()->DrawUTF8(10, 10, "↑");
    OLED_DEVICES()->DrawUTF8(4, 24, "退出");
    OLED_DEVICES()->DrawUTF8(4, 89, "切换");
    OLED_DEVICES()->DrawUTF8(10, 104, "↓");
  }

  OLED_SEND_BUFFER();
}

// 退出事件
void AppWindows::Out() {
}

void AppWindows::SwitchMode(uint8_t mode) {
  if (mode == 0) {
    if (m_model == 'W') {
      m_model = 'M';
    } else if (m_model == 'M') {
      m_model = 'W';
    } else {
      m_model = 'W';
    }
  } else {
    m_model = mode;
  }
}

uint8_t AppWindows::GetMode() {
  return m_model;
}