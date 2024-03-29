//
// Created by 19254 on 2023/8/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_APP_WINDOWS_HPP
#define HELLOWORD_DYNAMIC_FW_APP_WINDOWS_HPP
#include "apps.hpp"
#include <string>

// 窗口切换引用
class AppWindows: public AppBase {
private:
  uint8_t m_model = 'M'; // W:windows M:mac

public:

    unsigned char IsSend=1;
    unsigned char IsPrepare = 0;

    AppWindows() { AppID = APPID_WINDOW; }

    // 全局注册后只会调用一次,用于初始化,自行处理静态数据
    void Init() override;

    // 进入事件
    void In() override;

    // 刷新事件
    void ReView() override;

    // 退出事件
    void Out() override;

    // 切换模式
    void SwitchMode(uint8_t mode=0);
    uint8_t GetMode();

};


#endif // HELLOWORD_DYNAMIC_FW_APP_WINDOWS_HPP
