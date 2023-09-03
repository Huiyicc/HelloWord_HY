//
// Created by 19254 on 2023/8/27.
//
#include "app_desktop.hpp"
#include "ctrl.hpp"
#include "SDK/resource.hpp"
#include "SDK/Easingcpp.hpp"
#include "os/ButtonPin.hpp"

void appDesktopButtonPinCallback(enum ButtonPinCallType type) {
    if (g_sysCtx->Apps.Status != APPID_DESKTOP) {
        return;
    }
    //// 下辈子在写这种代码老子就是狗（焯
    //auto info = ((AppDesktop *) &(g_sysCtx->Apps.Apps[APPID_DESKTOP]))->GetDesktopInfo();
    auto info = ((AppDesktop *)(g_sysCtx->Apps.AppsMap[APPID_DESKTOP]))->GetDesktopInfo();
    switch (type) {
        case ButtonPinCallType::RightButtonPressed:
            AppChange(info->AppID);
            break;
        default:
            break;
    }
}

void EasingCallBack(
        double schedule,    //当前进度
        double cValue,        //当前缓动值
        int totalframes,    //总帧数
        int nframe,            //当前帧数
        bool &stop,            //是否停止
        int param1,
        void *param2) {
    auto _this = (AppDesktop *) param2;
    if (param1 == 0) {
        _this->SetShifting(float(0) + float(43) * cValue);
    } else {
        _this->SetShifting(float(0) - float(43) * cValue);
    }
    _this->ReView();
}

void KNobCallback(KnobStatus *status) {
    if (g_sysCtx->Apps.Status != APPID_DESKTOP) {
        return;
    }

    auto _this = ((AppDesktop *)(g_sysCtx->Apps.AppsMap[APPID_DESKTOP]));
    _this->m_knobStatus = status;
    // 计算偏移
    //auto shifting = fmod(status->PositionRaw, g_sysCtx->Device.ctrl.knob.encoderDistance);
    //_this->SetShifting(36 * (shifting / g_sysCtx->Device.ctrl.knob.encoderDistance));
    if (status->EncoderPosition != status->LastEncoderPosition) {
        if (status->EncoderPosition > status->LastEncoderPosition) {
            Easing_Strat(EasingType_Quadratic_Out, 50, 5, 0, 1, EasingCallBack, false, 1, (void *) _this);
            _this->SetShifting(0);
            _this->MenuUp();
        } else {
            Easing_Strat(EasingType_Quadratic_Out, 50, 5, 0, 1, EasingCallBack, false, 0, (void *) _this);
            _this->SetShifting(0);
            _this->MenuDown();
        }

    }
    _this->ReView();
}

//初始化事件
void AppDesktop::Init() {
//    m_menuList = (std::vector<desktopCallType> *) pvPortMalloc(sizeof(std::vector<desktopCallType>));
//    m_menuList = new(m_menuList) std::vector<desktopCallType>();
//
//    m_menuList->push_back({"音量",
//                           "0",
//                           APPID_VOLUME});
//    m_menuList->push_back({"滚动",
//                           "3",
//                           APPID_UPDOWN});
//    m_menuList->push_back({"亮度",
//                           "1",
//                           APPID_LIGHT});
//    m_menuList->push_back({"视窗",
//                           "4",
//                           APPID_WINDOW});
    // 不要小于3个,虽然做了处理,但是没测试,会有bug
    m_menuNum = 4;
    m_menuList = (desktopCallType *) pvPortMalloc(sizeof(desktopCallType) * m_menuNum);
    auto tmp = new ((desktopCallType*)m_menuList + 0) desktopCallType{"音量",
                                                                     "0",
                                                                     APPID_VOLUME};
    tmp = new ((desktopCallType*)m_menuList + 1) desktopCallType{"滚动",
                                                                 "3",
                                                                 APPID_UPDOWN};
    tmp = new ((desktopCallType*)m_menuList + 2) desktopCallType{"亮度",
                                                                 "1",
                                                                 APPID_LIGHT};
    tmp = new ((desktopCallType*)m_menuList + 3) desktopCallType{"视窗",
                                                                 "4",
                                                                 APPID_WINDOW};

    RegisterKNobCallback(KNobCallback);
    RegisterButtonPinCall(appDesktopButtonPinCallback);
}

// 进入事件
void AppDesktop::In() {
    g_sysCtx->Device.ctrl.knob.SetEncoderModePos(m_dividesNum);
    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_ENCODER);
    ReView();
}

// 刷新事件
void AppDesktop::ReView() {
    auto s = m_knobStatus == nullptr ? KnobStatus() : *m_knobStatus;
    OLED_CLEAR_BUFFER();
    int y = 20;
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->DrawBox(0, 58, 32, 32);
    y += m_shifting;

    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(u8g2_font_wqy12_t_gb2312a);
    OLED_DEVICES()->DrawUTF8(4, 0, m_menuList[m_menuIndex].name);

    OLED_DEVICES()->SetDrawColor(2);
    OLED_DEVICES()->SetFont(hwd_hy_U24);
    unsigned char maxs = 0;
    auto arrs = getIndexArr(&maxs);
    for (unsigned i=0;i<maxs;i++) {
        OLED_DEVICES()->DrawStr(4, y, m_menuList[arrs[i]].icon);
        y += 43;
    }
    OLED_SEND_BUFFER();
}

// 退出事件
void AppDesktop::Out() {

}

unsigned char* AppDesktop::getIndexArr(unsigned char*size) {
    auto arr=(unsigned char*) pvPortMalloc(sizeof(unsigned char) * 4);
    memset(arr, 0, sizeof(unsigned char) * 4);
    // 不足3个
    if (m_menuNum < 3) {
        *size = char(m_menuNum);
        for (int i = 0; i < 3; i++) {
            arr[i] = i+1>=m_menuNum?0:i;
        }
        return arr;
    }
    unsigned char i = 0;
    // 当前为第一个
    if (m_menuIndex == 0) {
        if (m_shifting > 0) {
            arr[i]=m_menuNum - 2;
            i++;
        }
        arr[i]=m_menuNum - 1;
        i++;
        arr[i]=0;
        i++;
        arr[i]=1;
        i++;
        if (m_shifting < 0) {
            arr[i]=2;
            i++;
        }
        *size=i;
        return arr;
    }

    // 当前为最后一个
    if (m_menuIndex == m_menuNum - 1) {
        if (m_shifting > 0) {
            arr[i]=m_menuNum - 3;
            i++;
        }
        arr[i]=m_menuNum - 2;
        i++;
        arr[i]=m_menuIndex;
        i++;
        arr[i]=0;
        i++;
        if (m_shifting < 0) {
            arr[i]=1;
            i++;
        }
        *size=i;
        return arr;
    }

    // 当前为中间
    if (m_shifting > 0) {
        if (m_menuIndex-1==0) {
            arr[i]=m_menuNum-1;
            i++;
        }else {
            arr[i]=m_menuIndex - 2;
            i++;
        }
    }
    arr[i]=m_menuIndex - 1;
    i++;
    arr[i]=m_menuIndex;
    i++;
    arr[i]=m_menuIndex + 1;
    i++;
    if (m_shifting < 0) {
        if (m_menuIndex+1==m_menuNum-1) {
            arr[i]=0;
        }else {
            arr[i]=m_menuIndex + 2;
        }
        i++;
    }
    *size=i;
    return arr;
}

void AppDesktop::SetShifting(float shifting) {
    m_shifting = shifting;
}

void AppDesktop::MenuUp() {
    auto s = m_menuNum - 1;
    if (m_menuIndex + 1 > s) {
        m_menuIndex = 0;
    }
    m_menuIndex++;
}

void AppDesktop::MenuDown() {
    if (m_menuIndex - 1 < 0) {
        m_menuIndex = m_menuNum - 1;
    }
    m_menuIndex--;
}

const desktopCallType *AppDesktop::GetDesktopInfo() {
    return &m_menuList[m_menuIndex];
}
