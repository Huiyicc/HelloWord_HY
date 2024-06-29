//
// Created by 19254 on 2023/8/30.
//

#include "app_hid.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"

bool hidApp = false;

void appHIDButtonPinCallback(enum ButtonPinCallType type) {
    if (g_sysCtx->Apps.Status != APPID_HID) {
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

void appHIDKNobCallback(bool move,KnobStatus *status) {
    if (g_sysCtx->Apps.Status != APPID_HID||!move) {
        return;
    }

}

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppHID::Init() {
    hidApp = false;
    RegisterButtonPinCall(appHIDButtonPinCallback);
    RegisterKNobCallback(appHIDKNobCallback);
};

// 进入事件
void AppHID::In() {
    hidApp = true;
    InitLastAgent();
    g_sysCtx->Device.ctrl.knob.SetEncoderModePos(12);
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_INTELLIGENT, nullptr);
    ReView();
};

// 刷新事件
void AppHID::ReView() {
    OLED_CLEAR_BUFFER();
    drawA();
    OLED_SEND_BUFFER();
};

// 绘制界面(无上位机版本)
void AppHID::drawA() {
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(font_default);
    OLED_DEVICES()->DrawUTF8(6, 30, "无");
    OLED_DEVICES()->DrawUTF8(6, 45, "上");
    OLED_DEVICES()->DrawUTF8(6, 60, "位");
    OLED_DEVICES()->DrawUTF8(6, 75, "机");
}

// 退出事件
void AppHID::Out() {
    hidApp = false;
};

