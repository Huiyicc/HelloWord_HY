//
// Created by 19254 on 2023/8/30.
//

#include "app_leftright.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"
#include "SDK/usbproto.hpp"
#include "storage.hpp"

void appLeftRightButtonPinCallback(enum ButtonPinCallType type) {
    if (g_sysCtx->Apps.Status != APPID_LEFTRIGHT) {
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

void appLeftRightKNobCallback(KnobStatus *status) {
    if (g_sysCtx->Apps.Status != APPID_LEFTRIGHT) {
        return;
    }
    auto ffa = status->Angle - lastAgent;
    if (fabs(ffa) > 10) {
        if (status->Angle > lastAgent) {
            HYSDK::USB::SendKeyBoardCode(0, HYSDK::USB::KeyCode_t::LEFT_ARROW);
        } else {
            HYSDK::USB::SendKeyBoardCode(0, HYSDK::USB::KeyCode_t::RIGHT_ARROW);
        }
        lastAgent = status->Angle;
    }
}

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppLeftRight::Init() {
    RegisterButtonPinCall(appLeftRightButtonPinCallback);
    RegisterKNobCallback(appLeftRightKNobCallback);
};

// 进入事件
void AppLeftRight::In() {
    InitLastAgent();
    auto& cfg = GetSysConfig()->apps;
    g_sysCtx->Device.ctrl.knob.SetEncoderModePos(cfg.LeftRight.EncodePos);
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t(cfg.LeftRight.Mode));
    ReView();
};

// 刷新事件
void AppLeftRight::ReView() {
    OLED_CLEAR_BUFFER();
    drawA();
    OLED_SEND_BUFFER();
};

// 绘制界面(无上位机版本)
void AppLeftRight::drawA() {
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->DrawUTF8(10, 10, "↑");
    OLED_DEVICES()->DrawUTF8(1, 24, "前进");
    OLED_DEVICES()->DrawUTF8(1, 89, "后退");
    OLED_DEVICES()->DrawUTF8(10, 104, "↓");
}

// 退出事件
void AppLeftRight::Out() {

};

