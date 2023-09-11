//
// Created by 19254 on 2023/8/30.
//

#include <string>
#include "app_light.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"


void appLightButtonPinCallback(enum ButtonPinCallType type) {
    if (g_sysCtx->Apps.Status != APPID_LIGHT) {
        return;
    }
    switch (type) {
        case ButtonPinCallType::LeftButtonLongPress:
            AppChange(APPID_DESKTOP);
            break;
        case ButtonPinCallType::RightButtonLongPress:{
//            g_sysCtx->Device.eink->DrawBitmap(gImage_1);
//            g_sysCtx->Device.eink->Update();
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


}

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppLight::Init() {
    RegisterButtonPinCall(appLightButtonPinCallback);
    RegisterKNobCallback(appLightKNobCallback);
};

// 进入事件
void AppLight::In() {
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_INERTIA);
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