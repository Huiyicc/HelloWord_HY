//
// Created by 19254 on 2023/8/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_APP_BASE_HPP
#define HELLOWORD_DYNAMIC_FW_APP_BASE_HPP

class AppBase {
protected:
    unsigned char AppID = 0;
public:
    // 全局注册后只会调用一次,用于初始化,自行处理静态数据
    virtual void Init() = 0;

    // 进入事件
    virtual void In() = 0;

    // 刷新事件
    virtual void ReView() = 0;

    // 退出事件
    virtual void Out() = 0;

    // 获取AppID
    unsigned char GetAppID() { return AppID; };
};


#endif //HELLOWORD_DYNAMIC_FW_APP_BASE_HPP
