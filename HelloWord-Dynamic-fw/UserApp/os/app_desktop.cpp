//
// Created by 19254 on 2023/8/27.
//
#include "app_desktop.hpp"
#include "ctrl.hpp"
#include "SDK/resource.hpp"

void KNobCallback(KnobStatus *status){
    auto _this=(AppDesktop*)g_sysCtx->Apps.AppsMap[APPID_DESKTOP];
    _this->m_knobStatus=status;
    _this->ReView();
}

//初始化事件
void AppDesktop::Init(){
    RegisterKNobCallback(KNobCallback);
}

// 进入事件
void AppDesktop::In() {
    g_sysCtx->Device.ctrl.knob.SetEncoderModePos(DividesNum);
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_ENCODER);
    ReView();
}

// 刷新事件
void AppDesktop::ReView() {
    auto s = m_knobStatus== nullptr?KnobStatus():*m_knobStatus;
    g_sysCtx->Device.oled->ClearBuffer();
    g_sysCtx->Device.oled->SetDrawColor(1);

    //g_sysCtx->Device.oled->DrawStr(0,0,std::to_string(s.Position).c_str());

    g_sysCtx->Device.oled->SendBuffer();
}

// 退出事件
void AppDesktop::Out() {

}