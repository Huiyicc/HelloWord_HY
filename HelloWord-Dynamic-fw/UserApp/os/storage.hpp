//
// Created by 19254 on 2023/9/17.
//

#ifndef STM32F4FLASH_STORAGE_H
#define STM32F4FLASH_STORAGE_H
#include "os_define.hpp"
#include "timers.h"

// 初始化存储区域
void InitStorage();
// 获取配置
OsConfig *GetSysConfig(bool update = false);
void StorageTask(TimerHandle_t t);

#endif //STM32F4FLASH_STORAGE_H
