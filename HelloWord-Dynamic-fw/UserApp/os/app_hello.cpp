//
// Created by 19254 on 2023/8/28.
//

#include "app_hello.hpp"
#include "SDK/OLED.hpp"
#include "ctrl.hpp"
#include "sleep.hpp"
#include "ButtonPin.hpp"

void sysInit() {
    g_sysCtx->Device.eink->Init();
    CtrlInit();
    ButtonPinInit();
    SleepTimerInit();
}

//初始化事件
void AppHello::Init() {

}

// 进入事件
void AppHello::In() {
    ReView();
}

void AppHello::drawLogo(int x,int y){
    g_sysCtx->Device.oled->SetDrawColor(1);
    g_sysCtx->Device.oled->DrawBox(x,y,20,20);
    g_sysCtx->Device.oled->SetDrawColor(0);
    g_sysCtx->Device.oled->DrawStr(x+3,y+4,"HY");
}

// 刷新事件
void AppHello::ReView() {
    auto& oled = g_sysCtx->Device.oled;
    oled->ClearBuffer();
    // logo区域
    drawLogo(6,11);
    // 文字区域
    oled->SetDrawColor(1);
    SDK::OLEDDrawUTF8Str_wqy12_STL(9, 41, "系统启动中", 0);
    oled->SendBuffer();
    sysInit();
}

// 退出事件
void AppHello::Out() {

}