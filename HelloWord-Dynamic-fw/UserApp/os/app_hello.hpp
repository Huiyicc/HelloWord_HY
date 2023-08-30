//
// Created by 19254 on 2023/8/28.
//

#ifndef HELLOWORD_DYNAMIC_FW_APP_HELLO_HPP
#define HELLOWORD_DYNAMIC_FW_APP_HELLO_HPP

#include "apps.hpp"

class AppHello : public AppBase {
private:
    void drawLogo(int x,int y);
public:
    AppHello() { AppID = APPID_HELLO; }

    // 全局注册后只会调用一次,用于初始化,自行处理静态数据
    void Init() override;

    // 进入事件
    void In() override;

    // 刷新事件
    void ReView() override;

    // 退出事件
    void Out() override;

};


#endif //HELLOWORD_DYNAMIC_FW_APP_HELLO_HPP
