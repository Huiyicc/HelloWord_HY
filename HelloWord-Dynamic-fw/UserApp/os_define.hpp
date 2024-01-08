//
// Created by 19254 on 2023/8/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_OS_DEFINE_HPP
#define HELLOWORD_DYNAMIC_FW_OS_DEFINE_HPP

#include "os/app_base.hpp"
#include <map>
#include "build_info.h"
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
    double Velocity = 0;
};

typedef void (*KnobCallback)(KnobStatus *status);

struct SysDeviceCtrl {
    bool Action = false;
    Encoder encoder = Encoder(&hspi1);
    Driver driver = Driver(12);
    Motor motor = Motor(7);
    KnobSimulator knob = KnobSimulator();
    HYSDK::CList<KnobCallback> CallBacks; //回调通知事件
};

enum RGBEffect {
    // 默认渐变
    Default_ = 0,
    // 轮询渐变
    Gradient,
};

struct SysDeviceRGB {
    RGB *rgb = nullptr;
    RGBEffect effect = RGBEffect::Default_;
};

struct SysDevice {
    BoardConfig_t *boardConfig = nullptr;
    SSD1306 *oled = nullptr;
    SysDeviceRGB rgb;
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
    unsigned char ButtonPinsSize = 0;
    HYSDK::CList<ButtonPinCall> ButtonPins;
};

extern SysContext *g_sysCtx;

// 系统调度事件
typedef int(*SysCallFunc)(SysContext *ctx);

// ========== config ==========

// 注意:
// 为了保证配置兼容,不允许调整成员顺序,不允许删除成员,只能增加成员
// 增删配置成员请先考虑清楚

// 电机配置
struct KnobConfig {
    float zeroPosition = -3.7f;
};

// 设备配置
struct DeviceConfig {
    KnobConfig knob;
};

// 应用电机反馈配置
struct AppKnobConfig {
    // 反馈模式
    int Mode = KnobSimulator::Mode_t::MODE_INTELLIGENT;
    // 编码器步数
    int EncodePos = 12;
    // 电机力矩限制
    float TorqueLimit = 0.4f;
    // 速度限制
    float VelocityLimit = 3;
    // 增量(例如重复触发n次)
    int AddedValue = 1;
};

// app
struct AppsConfig {
    // 音量调节
    AppKnobConfig Volume;
    // 上下滚动
    AppKnobConfig UpDown;
    // 左右滚动
    AppKnobConfig LeftRight;
    // 亮度调节
    AppKnobConfig Light;
};

struct SysConfig {
    // 休眠时间,单位:秒
    int SleepTime = 10;
};

struct OsConfig {
    DeviceConfig devices;
    AppsConfig apps;
    SysConfig sys;
};

extern OsConfig g_SysConfig;

// ========== config ==========
#define font_default u8g2_font_wqy12_t_gb2312

#ifndef SLEEPID_BUTTONPIN
#define SLEEPID_BUTTONPIN 1
#endif

#ifndef OLED_CLEAR_BUFFER
#define OLED_CLEAR_BUFFER() {g_sysCtx->Device.oled->ClearBuffer();g_sysCtx->Device.oled->SetFont(font_default);}
#endif
#ifndef OLED_SEND_BUFFER
#define OLED_SEND_BUFFER() g_sysCtx->Device.oled->SendBuffer();
#endif

#ifndef OLED_DEVICES
#define OLED_DEVICES() g_sysCtx->Device.oled
#endif

void HID_GetKnobStatus();

#endif //HELLOWORD_DYNAMIC_FW_OS_DEFINE_HPP
