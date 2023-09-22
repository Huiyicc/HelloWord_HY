//
// Created by 19254 on 2023/9/17.
//

#ifndef STM32F4FLASH_STORAGE_H
#define STM32F4FLASH_STORAGE_H
#include "os_define.hpp"

// 初始化存储区域
void InitStorage();
SysConfig *GetSysConfig(bool update = false);

#endif //STM32F4FLASH_STORAGE_H
