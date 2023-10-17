//
// Created by 19254 on 2023/8/28.
//

#ifndef HELLOWORD_DYNAMIC_FW_APPS_HPP
#define HELLOWORD_DYNAMIC_FW_APPS_HPP

#include "os_define.hpp"

#ifndef APPID_DESKTOP
#define APPID_DESKTOP 1
#endif
#ifndef APPID_VOLUME
#define APPID_VOLUME 2
#endif
#ifndef APPID_UPDOWN
#define APPID_UPDOWN 3
#endif
#ifndef APPID_LIGHT
#define APPID_LIGHT 4
#endif
#ifndef APPID_WINDOW
#define APPID_WINDOW 5
#endif
#ifndef APPID_LEFTRIGHT
#define APPID_LEFTRIGHT 6
#endif
#ifndef APPID_HID
#define APPID_HID 7
#endif

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

// 取消注册
inline void AppUnRegister(unsigned char appid) {
    auto iter = g_sysCtx->Apps.AppsMap.find(appid);
    if (iter == g_sysCtx->Apps.AppsMap.end()) {
        return;
    }
    vPortFree(iter->second);
    g_sysCtx->Apps.AppsMap.erase(iter);
}

// 切换App
inline void AppChange(unsigned char appid) {
    if (appid==g_sysCtx->Apps.Status) {
        g_sysCtx->Apps.AppsMap[g_sysCtx->Apps.Status]->ReView();
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
//inline void AppChange(unsigned char appid) {
//    AppBase* newApp = nullptr;
//    for (unsigned char i = 0; i < g_sysCtx->Apps.AppSize; ++i) {
//        if (g_sysCtx->Apps.Apps[i].GetAppID()==appid) {
//            newApp = &g_sysCtx->Apps.Apps[i];
//            break;
//        }
//    }
//    g_sysCtx->Apps.Apps[g_sysCtx->Apps.Status].Out();
//    g_sysCtx->Apps.Status = appid;
//    newApp->In();
//}

#endif //HELLOWORD_DYNAMIC_FW_APPS_HPP
