//
// Created by 19254 on 2023/8/30.
//

#include <string>
#include "app_light.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "SDK/utils.hpp"


void appLightButtonPinCallback(enum ButtonPinCallType type) {
    if (g_sysCtx->Apps.Status != APPID_LIGHT) {
        return;
    }
    switch (type) {
        case ButtonPinCallType::LeftButtonPressed:
            AppChange(APPID_DESKTOP);
            break;
        case ButtonPinCallType::RightButtonLongPress: {
        }
            break;
        default:
            break;
    }
}

void appLightKNobCallback(KnobStatus *status) {
    if (g_sysCtx->Apps.Status != APPID_LIGHT) {
        return;
    }
	auto x = status->PositionRaw - status->LastPositionRaw;
	if (fabs(x) < 0.1) {
		return;
	}
	auto _this = ((AppLight *) (g_sysCtx->Apps.AppsMap[APPID_LIGHT]));
	if (status->Position > status->LastPosition) {
		_this->LightUP();
	} else {
		_this->LightDOWN();
	}
//    if (lastAgent == 0) { lastAgent=status->Angle;return; }
//    auto _this = ((AppLight *) (g_sysCtx->Apps.AppsMap[APPID_LIGHT]));
//    auto ffa = status->Angle - lastAgent;
//    if (fabs(ffa) > 14) {
//        if (status->Angle > lastAgent) {
//            _this->LightUP();
//        } else {
//            _this->LightDOWN();
//        }
//        lastAgent = status->Angle;
//    }
}

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppLight::Init() {
    RegisterButtonPinCall(appLightButtonPinCallback);
    RegisterKNobCallback(appLightKNobCallback);
};

// 进入事件
void AppLight::In() {
    InitLastAgent();
    g_sysCtx->Device.ctrl.knob.SetEncoderModePos(12);
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_INTELLIGENT);
    ReView();
};

// 刷新事件
void AppLight::ReView() {
    OLED_CLEAR_BUFFER();
    drawA();
    OLED_SEND_BUFFER();
};

// 绘制界面(无上位机版本)
void AppLight::drawA() {
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->DrawUTF8(10, 10, "↑");
    OLED_DEVICES()->DrawUTF8(10, 24, "UP");
    OLED_DEVICES()->DrawUTF8(4, 58, "亮度");
    OLED_DEVICES()->DrawUTF8(10, 89, "DN");
    OLED_DEVICES()->DrawUTF8(10, 104, "↓");
}

// 退出事件
void AppLight::Out() {

};

void AppLight::LightUP() {
    //亮度加
    uint8_t HID_report[9] = {0};
    HID_report[0] = 0x05;
    HID_report[1] = 0x6F;
    osDelay(1);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_report, 9);
    osDelay(1);
    HID_report[0] = 0x05;
    HID_report[1] = 0x00;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_report, 9);
}

void AppLight::LightDOWN() {
    //亮度减
    uint8_t HID_report[9] = {0};
    HID_report[0] = 0x05;
    HID_report[1] = 0x70;
    osDelay(1);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_report, 9);
    osDelay(1);
    HID_report[0] = 0x05;
    HID_report[1] = 0x00;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_report, 9);
}