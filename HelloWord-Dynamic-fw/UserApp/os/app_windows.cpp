//
// Created by 19254 on 2023/8/27.
//
#include "app_windows.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"
#include "SDK/usbproto.hpp"
#include "SDK/utils.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"

void appWindowsButtonPinCallback(enum ButtonPinCallType type){
    if (g_sysCtx->Apps.Status != APPID_WINDOW) {
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

void appWindowsKNobCallback(KnobStatus *status) {
    if (g_sysCtx->Apps.Status != APPID_WINDOW) {
        return;
    }
    auto _this = ((AppWindows *) (g_sysCtx->Apps.AppsMap[APPID_WINDOW]));
    uint8_t keyBoardData[9] = {2,0,0,0,0,0,0,0};
    uint8_t keyBoardDataEmpty[9] = {2,0,0,0,0,0,0,0};
    auto x = fabs(status->PositionRaw);
    if (fabs(x) < 0.8 // 位置小于防抖区间
    && _this->IsSend==0 // 从未发送按键信息
    && _this->IsPrepare!=0 // 上次位置超过上报准备区
    ) {
        // 上报按键信息
        switch (_this->IsPrepare) {
            case 1:
                // 上推
                // LWIN+TAB
                keyBoardData[1] = ((1 << ((0x84 & 0x0f) - 1)));
                keyBoardData[3] = HYSDK::USB::KeyCode_t::TAB;
                USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&keyBoardData,9);
                HAL_Delay(3);
                USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&keyBoardDataEmpty,9);
                HAL_Delay(3);

                break;
            case 2:
                // 下拉
                // LWIN+D
                keyBoardData[1] = ((1 << ((0x84 & 0x0f) - 1)));
                keyBoardData[3] = HYSDK::USB::KeyCode_t::D;
                USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&keyBoardData,9);
                HAL_Delay(3);
                USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&keyBoardDataEmpty,9);
                HAL_Delay(3);

                break;
            default:
                break;
        }
        _this->IsSend==1;
        _this->IsPrepare = 0;
        return;

    }
    if (_this->IsPrepare!=0&&_this->IsSend==0) {
        return;
    }
    auto m = fmod(status->PositionRaw,_2PI);
    if (m > 0.7) {
        // 开启上推
        _this->IsPrepare = 1;
        _this->IsSend = 0;
        return;

    } else if (m < -0.7) {
        // 开启下拉
        _this->IsPrepare = 2;
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
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_PADDLE);
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
