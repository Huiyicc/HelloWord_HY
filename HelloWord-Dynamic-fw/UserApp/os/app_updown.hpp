//
// Created by 19254 on 2023/8/30.
//

#ifndef HELLOWORD_DYNAMIC_FW_APPUPDOWN_HPP
#define HELLOWORD_DYNAMIC_FW_APPUPDOWN_HPP
#include "apps.hpp"

class AppUpDown: public AppBase {
private:
    void drawA();

public:
    AppUpDown() { AppID = APPID_UPDOWN; }

    // 全局注册后只会调用一次,用于初始化,自行处理静态数据
    void Init() override;

    // 进入事件
    void In() override;

    // 刷新事件
    void ReView() override;

    // 退出事件
    void Out() override;
};


#endif //HELLOWORD_DYNAMIC_FW_APPUPDOWN_HPP
