#include "common_inc.h"
#include "os/apps.hpp"
#include "os/app_desktop.hpp"
#include "os/app_volume.hpp"
#include "os/app_updown.hpp"
#include "os/app_light.hpp"
#include "os/app_windows.hpp"
#include "os/app_leftright.hpp"
#include "os/app_hid.hpp"
#include "os/ctrl.hpp"
#include "os/sleep.hpp"
#include "os/ButtonPin.hpp"
#include "os/RGB.hpp"
#include "SDK/utils.hpp"
#include "os/storage.hpp"

SysContext *g_sysCtx = nullptr;

void SysTask(TimerHandle_t t) {
    RGBTask();
    ButtonTask(t);
    timerSleepCallback(t);
}

template<class T>
void regApp() {
    auto lAppPtr = (T *) pvPortMalloc(sizeof(T));
    lAppPtr = new(lAppPtr) T();
    AppRegister(lAppPtr);
}

void helloWord() {
    g_sysCtx->Device.eink->Init();
    g_sysCtx->Device.eink->demo();

    //g_sysCtx->Device.eink->DeepSleep();
    CtrlInit();
    ButtonPinInit();
    //SleepTimerInit();
    xTimerStart(xTimerCreate("SysTimer", pdMS_TO_TICKS(100), pdTRUE, nullptr, SysTask), 0);
    if (!g_sysCtx->Device.ctrl.Action) {
        OLED_CLEAR_BUFFER();
        // logo区域
        OLED_DEVICES()->SetDrawColor(1);
        OLED_DEVICES()->DrawBox(6, 11, 20, 20);
        OLED_DEVICES()->DrawUTF8(9, 41, "校");
        OLED_DEVICES()->DrawUTF8(9, 55, "准");
        OLED_DEVICES()->DrawUTF8(9, 69, "失");
        OLED_DEVICES()->DrawUTF8(9, 83, "败");
        OLED_DEVICES()->SetDrawColor(0);
        OLED_DEVICES()->DrawStr(6 + 3, 15, "HY");
        OLED_SEND_BUFFER();
        exit(1);
    }


}

void InitOs() {
    //初始化所有设备
    g_sysCtx = (SysContext *) pvPortMalloc(sizeof(SysContext));
    g_sysCtx = new(g_sysCtx) SysContext();

    g_sysCtx->Device.boardConfig = (BoardConfig_t *) pvPortMalloc(sizeof(BoardConfig_t));
    g_sysCtx->Device.boardConfig = new(g_sysCtx->Device.boardConfig) BoardConfig_t();

    g_sysCtx->Device.oled = (SSD1306 *) pvPortMalloc(sizeof(SSD1306));
    g_sysCtx->Device.oled = new(g_sysCtx->Device.oled) SSD1306(&hi2c1);
    g_sysCtx->Device.oled->Init();

    OLED_CLEAR_BUFFER();
    // logo区域
    g_sysCtx->Device.oled->SetDrawColor(1);
    g_sysCtx->Device.oled->DrawBox(6, 11, 20, 20);
    // 文字区域
    OLED_DEVICES()->DrawUTF8(9, 41, "系");
    OLED_DEVICES()->DrawUTF8(9, 55, "统");
    OLED_DEVICES()->DrawUTF8(9, 69, "启");
    OLED_DEVICES()->DrawUTF8(9, 83, "动");
    OLED_DEVICES()->DrawUTF8(9, 97, "中");
    g_sysCtx->Device.oled->SetDrawColor(0);
    g_sysCtx->Device.oled->DrawStr(6 + 3, 15, "HY");
    OLED_SEND_BUFFER();
    osDelay(1500);
    // 初始化存储
    InitStorage();

    RGBInit();

    g_sysCtx->Device.eink = (Eink290BW *) pvPortMalloc(sizeof(Eink290BW));
    g_sysCtx->Device.eink = new(g_sysCtx->Device.eink) Eink290BW();

    // APP在这注册
    regApp<AppDesktop>();
    regApp<AppWindows>();
    regApp<AppVolume>();
    regApp<AppUpDown>();
    regApp<AppLight>();
    regApp<AppLeftRight>();
    regApp<AppHID>();
    // 此处初始化所有APP
    for (auto &iter: g_sysCtx->Apps.AppsMap) {
        iter.second->Init();
    }

    //启动事件
    helloWord();
    AppChange(APPID_DESKTOP);
}

void Main(void) {
    InitOs();
}

