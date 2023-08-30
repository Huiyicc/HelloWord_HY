//
// Created by 19254 on 2023/8/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_APP_DESKTOP_HPP
#define HELLOWORD_DYNAMIC_FW_APP_DESKTOP_HPP
#include "apps.hpp"
#include <string>

struct desktopCallType {
    std::string name;
    void *icon;
    int AppID;
};

// 桌面应用
class AppDesktop: public AppBase {
private:
    static std::vector<desktopCallType> *m_menuList;
    int DividesNum = 10;
public:
    KnobStatus* m_knobStatus = nullptr;

    AppDesktop() { AppID = APPID_DESKTOP; }

    // 全局注册后只会调用一次,用于初始化,自行处理静态数据
    void Init() override;

    // 进入事件
    void In() override;

    // 刷新事件
    void ReView() override;

    // 退出事件
    void Out() override;
};


#endif //HELLOWORD_DYNAMIC_FW_APP_DESKTOP_HPP
