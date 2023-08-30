//
// Created by 19254 on 2023/8/28.
//

#ifndef HELLOWORD_DYNAMIC_FW_APPS_HPP
#define HELLOWORD_DYNAMIC_FW_APPS_HPP

#include "os_define.hpp"

// 注册应用
// 注意:不要重复
inline bool AppRegister(AppBase *app) {
    if (!app) {
        return false;
    }
    if (app->GetAppID() < 1) {
        return false;
    }
    auto iter = g_sysCtx->Apps.AppsMap.find(app->GetAppID());
    if (iter == g_sysCtx->Apps.AppsMap.end()) {
        g_sysCtx->Apps.AppsMap[app->GetAppID()] = app;
        return true;
    }
    return false;
}

// 切换App
inline void AppChange(unsigned char appid) {
    if (appid < 1) {
        return;
    }
    auto iter = g_sysCtx->Apps.AppsMap.find(appid);
    if (iter == g_sysCtx->Apps.AppsMap.end()) { return; }
    if (g_sysCtx->Apps.Status > 0) {
        g_sysCtx->Apps.AppsMap[g_sysCtx->Apps.Status]->Out();
    }
    g_sysCtx->Apps.Status = appid;
    iter->second->In();
}


#endif //HELLOWORD_DYNAMIC_FW_APPS_HPP
