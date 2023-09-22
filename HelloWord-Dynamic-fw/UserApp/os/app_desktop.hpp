//
// Created by 19254 on 2023/8/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_APP_DESKTOP_HPP
#define HELLOWORD_DYNAMIC_FW_APP_DESKTOP_HPP
#include "apps.hpp"
#include "SDK/Easingcpp.hpp"
#include <string>

struct desktopCallType {
    const char* name;
    const char* icon;
    int AppID;
};

// 桌面应用
class AppDesktop: public AppBase {
private:
    unsigned char m_status=0; // 0:正常 1:上移 2:下移
    unsigned char m_menuIndex = 0;
    //std::vector<desktopCallType> *m_menuList;
    unsigned char m_menuNum = 4;
    desktopCallType* m_menuList= nullptr;
    unsigned char m_dividesNum = 10;

    float m_shifting = 0;
    unsigned char* getIndexArr(unsigned char*size);
public:
    char EasingType = EasingType_CircularCurve_Out;

    AppDesktop() { AppID = APPID_DESKTOP; }

    // 全局注册后只会调用一次,用于初始化,自行处理静态数据
    void Init() override;

    // 进入事件
    void In() override;

    // 刷新事件
    void ReView() override;

    // 退出事件
    void Out() override;

    void SetShifting(float shifting);

    void MenuDec();

    void MenuAdd();

    unsigned char GetMenuIndex();
    const float* GetShifting(){return &m_shifting;};

    const desktopCallType* GetDesktopInfo();

};


#endif //HELLOWORD_DYNAMIC_FW_APP_DESKTOP_HPP
