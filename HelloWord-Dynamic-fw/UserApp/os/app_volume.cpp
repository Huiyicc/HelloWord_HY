//
// Created by 19254 on 2023/8/30.
//

#include <string>
#include "app_volume.hpp"
#include "ButtonPin.hpp"
#include "ctrl.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"


void appVolumeButtonPinCallback(enum ButtonPinCallType type){
    if (g_sysCtx->Apps.Status != APPID_VOLUME) {
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

void appVolumeKNobCallback(KnobStatus *status) {
    if (g_sysCtx->Apps.Status != APPID_VOLUME) {
        return;
    }
    auto _this = ((AppVolume *)(g_sysCtx->Apps.AppsMap[APPID_VOLUME]));
    if (status->Position > status->LastPosition) {
        _this->VolumeDOWN();
    } else {
        _this->VolumeUP();
    }
}

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppVolume::Init() {
    RegisterButtonPinCall(appVolumeButtonPinCallback);
    RegisterKNobCallback(appVolumeKNobCallback);
};

// 进入事件
void AppVolume::In() {
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_INERTIA);
    ReView();
};

// 刷新事件
void AppVolume::ReView() {
    OLED_CLEAR_BUFFER();
    drawA();
    OLED_SEND_BUFFER();
};

// 绘制界面(无上位机版本)
void AppVolume::drawA() {
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->DrawUTF8(10, 10, "↑");
    OLED_DEVICES()->DrawUTF8(10, 24, "UP");
    OLED_DEVICES()->DrawUTF8(6, 58, "音量");
    OLED_DEVICES()->DrawUTF8(10, 89, "DN");
    OLED_DEVICES()->DrawUTF8(10, 104, "↓");
}

// 退出事件
void AppVolume::Out() {

};

void AppVolume::VolumeUP() {
    //音量减
    uint8_t   HID_report[4];
    HID_report[0]=0x02;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,HID_report,1);
    HAL_Delay(20);
    HID_report[0]=0x00;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,HID_report,1);

}
void AppVolume::VolumeDOWN() {
    //音量加
    uint8_t   HID_report[4];
    HID_report[0]=0x01;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,HID_report,1);
    HAL_Delay(20);
    HID_report[0]=0x00;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,HID_report,1);

}