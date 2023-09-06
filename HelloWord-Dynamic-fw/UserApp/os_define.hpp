//
// Created by 19254 on 2023/8/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_OS_DEFINE_HPP
#define HELLOWORD_DYNAMIC_FW_OS_DEFINE_HPP

#include "os/app_base.hpp"
#include <map>
#include "SDK/CList.hpp"
#include "common_inc.h"
#include "Platform/Sensor/Encoder/encoder.h"
#include "Platform/Driver/driver.h"
#include "Ctrl/Motor/motor.h"
#include "Ctrl/Motor/knob.h"
#include "eink_290_bw.h"
#include "U8g2lib.hpp"
#include "rgb_light.h"

enum ButtonPinCallType {
    // 左侧按钮短按事件
    LeftButtonPressed = 1,
    // 右侧按钮短按事件
    RightButtonPressed = 2,
    // 左侧按钮长按事件
    LeftButtonLongPress = 3,
    // 右侧按钮长按事件
    RightButtonLongPress = 4,
    Default = 0,
};

typedef void(*ButtonPinCall)(enum ButtonPinCallType type);

struct KnobStatus {
    //原始数据
    float PositionRaw = 0;
    float LastPositionRaw = 0;

    // 当前位置
    float Position = 0;
    // 上次位置
    float LastPosition = 0;

    int LastEncoderPosition = 0;
    int EncoderPosition = 0;

    // 角度
    double Angle = 0;
};

typedef void (*KnobCallback)(KnobStatus *status);

struct SysDeviceCtrl {
    Encoder encoder = Encoder(&hspi1);
    Driver driver = Driver(12);
    Motor motor = Motor(7);
    KnobSimulator knob = KnobSimulator();
    HYSDK::CList<KnobCallback> CallBacks; //回调通知事件
};

struct SysDevice {
    BoardConfig_t *boardConfig = nullptr;
    SSD1306 *oled = nullptr;
    RGB *rgb = nullptr;
    Eink290BW *eink = nullptr;
    SysDeviceCtrl ctrl;
};


struct SysApps {
    // 当前App
    unsigned char Status = 0;
    // App列表
    std::map<unsigned char, AppBase *> AppsMap;
    //unsigned char AppSize=0;
    //AppBase* Apps = nullptr;
};

struct SysContext {
    SysDevice Device;
    SysApps Apps;
    // 按钮回调事件
    unsigned char ButtonPinsSize=0;
    HYSDK::CList<ButtonPinCall> ButtonPins;
};

extern SysContext *g_sysCtx;

// 系统调度事件
typedef int(*SysCallFunc)(SysContext *ctx);


#ifndef SLEEPID_BUTTONPIN
#define SLEEPID_BUTTONPIN 1
#endif

#ifndef OLED_CLEAR_BUFFER
#define OLED_CLEAR_BUFFER() {g_sysCtx->Device.oled->ClearBuffer();g_sysCtx->Device.oled->SetFont(u8g2_font_wqy12_t_gb2312a);}
#endif
#ifndef OLED_SEND_BUFFER
#define OLED_SEND_BUFFER() g_sysCtx->Device.oled->SendBuffer();
#endif

#ifndef OLED_DEVICES
#define OLED_DEVICES() g_sysCtx->Device.oled
#endif

#endif //HELLOWORD_DYNAMIC_FW_OS_DEFINE_HPP