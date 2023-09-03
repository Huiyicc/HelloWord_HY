//
// Created by 19254 on 2023/8/27.
//
#include "app_windows.hpp"
#include "ButtonPin.hpp"

void appWindowsButtonPinCallback(enum ButtonPinCallType type){
    if (g_sysCtx->Apps.Status != APPID_WINDOW) {
        return;
    }
    switch (type) {
        case ButtonPinCallType::LeftButtonLongPress:
            AppChange(APPID_DESKTOP);
            break;
        default:
            break;
    }
}

//初始化事件
void AppWindows::Init() {
    RegisterButtonPinCall(appWindowsButtonPinCallback);
}

// 进入事件
void AppWindows::In() {
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_DAMPED);
    ReView();
}

// 刷新事件
void AppWindows::ReView() {
    OLED_CLEAR_BUFFER();
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(u8g2_font_wqy12_t_gb2312a);
    OLED_DEVICES()->DrawUTF8(10, 10, "↑");
    OLED_DEVICES()->DrawUTF8(4, 24, "任务");
    OLED_DEVICES()->DrawUTF8(4, 38, "视图");
    OLED_DEVICES()->DrawUTF8(4, 89, "桌面");
    OLED_DEVICES()->DrawUTF8(10, 104, "↓");
    OLED_SEND_BUFFER();
}

// 退出事件
void AppWindows::Out() {

}
