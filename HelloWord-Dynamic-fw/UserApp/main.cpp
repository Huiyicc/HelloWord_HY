#include "os/apps.hpp"
#include "os/app_desktop.hpp"
#include "os/app_volume.hpp"
#include "os/app_updown.hpp"
#include "os/app_light.hpp"
#include "os/app_windows.hpp"
#include "usbd_customhid.h"
#include "usb_device.h"
#include "os/app_windows.hpp"
#include "os/ctrl.hpp"
#include "os/sleep.hpp"
#include "os/ButtonPin.hpp"

SysContext *g_sysCtx = nullptr;

void SysTask(TimerHandle_t t) {
    ButtonTask(t);
    timerSleepCallback(t);
}

//template<class T>
//void regApp(void *ptr, unsigned char Appid) {
//    auto lAppPtr = (T *) pvPortMalloc(sizeof(T));
//    lAppPtr= new(lAppPtr) T();
//    AppRegister(lAppPtr);
//}
template<class T>
void regApp() {
    auto lAppPtr = (T *) pvPortMalloc(sizeof(T));
    lAppPtr= new(lAppPtr) T();
    AppRegister(lAppPtr);
}

void helloWord() {
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
    g_sysCtx->Device.eink->Init();
    CtrlInit();
    ButtonPinInit();
    //SleepTimerInit();
    xTimerStart(xTimerCreate("SysTimer", pdMS_TO_TICKS(100), pdTRUE, nullptr, SysTask), 0);
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

    g_sysCtx->Device.rgb = (RGB *) pvPortMalloc(sizeof(RGB));
    g_sysCtx->Device.rgb = new(g_sysCtx->Device.rgb) RGB(&hspi3);

    g_sysCtx->Device.eink = (Eink290BW *) pvPortMalloc(sizeof(Eink290BW));
    g_sysCtx->Device.eink = new(g_sysCtx->Device.eink) Eink290BW();

    // APP在这注册
    //regApp<AppHello>();
    regApp<AppDesktop>();
    regApp<AppWindows>();
    regApp<AppVolume>();
    regApp<AppUpDown>();
    regApp<AppLight>();
    // 此处初始化所有APP
    for (auto &iter: g_sysCtx->Apps.AppsMap) {
        iter.second->Init();
    }
//    g_sysCtx->Apps.AppSize = 5;
//    g_sysCtx->Apps.Apps = (AppBase *) pvPortMalloc(sizeof(AppBase) * g_sysCtx->Apps.AppSize);
//    regApp<AppDesktop>(g_sysCtx->Apps.Apps, APPID_DESKTOP);
//    regApp<AppWindows>(g_sysCtx->Apps.Apps, APPID_WINDOW);
//    regApp<AppVolume>(g_sysCtx->Apps.Apps, APPID_VOLUME);
//    regApp<AppUpDown>(g_sysCtx->Apps.Apps, APPID_UPDOWN);
//    regApp<AppLight>(g_sysCtx->Apps.Apps, APPID_LIGHT);
//    // 此处初始化所有APP
//    for (int i = 0; i < g_sysCtx->Apps.AppSize; i++) {
//        g_sysCtx->Apps.Apps[i].Init();
//    }

    //启动事件
    //AppChange(APPID_HELLO);
    helloWord();
    AppChange(APPID_DESKTOP);
    //AppChange(APPID_VOLUME);
}

/* Default Entry -------------------------------------------------------*/
void Main(void) {
    //InitCommunication();
    InitOs();
}

//
extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    g_sysCtx->Device.rgb->isRgbTxBusy = false;
}