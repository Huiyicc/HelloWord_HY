#include "os/apps.hpp"
#include "os/app_hello.hpp"
#include "os/app_desktop.hpp"
#include "usbd_customhid.h"
#include "usb_device.h"

SysContext *g_sysCtx = nullptr;

template<class T>
void regApp() {
    auto lAppPtr = (T *) pvPortMalloc(sizeof(T));
    lAppPtr= new(lAppPtr) T();
    AppRegister(lAppPtr);
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
    regApp<AppHello>();
    regApp<AppDesktop>();


    // 此处初始化所有APP
    for (auto& iter:g_sysCtx->Apps.AppsMap) {
        iter.second->Init();
    }

    //启动事件
    AppChange(APPID_HELLO);
    AppChange(APPID_DESKTOP);

}

/* Default Entry -------------------------------------------------------*/
void Main(void) {
    InitCommunication();
    InitOs();

}

//
extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    g_sysCtx->Device.rgb->isRgbTxBusy = false;
}