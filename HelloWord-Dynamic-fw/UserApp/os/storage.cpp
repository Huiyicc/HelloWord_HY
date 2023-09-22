//
// Created by 19254 on 2023/9/17.
//
#include <cstring>
#include "storage.hpp"
#include "storage_compatible.hpp"
#include "stm32f4xx_hal.h"

bool m_updateStatus = false;
SysConfig g_SysConfig;

SysConfig *GetSysConfig(bool update) {
    m_updateStatus = update;
    return &g_SysConfig;
}


// =====================

// 数据区总大小
#define FLASH_SECTOR_SIZE 131072
// 参数区
// 数据使用 0x68,0x79 双字节标识
// 存储区域首指针寻址(扇区10)
// { 0x68,0x79,偏移数据}
#define ADDR_FLASH_SECTOR_10 ((uint32_t) 0x080C0000)
// 存储区域起始指针(扇区11)
// { 0x68,0x79,4字节配置长度 {},配置数据}
#define ADDR_FLASH_SECTOR_11 ((uint32_t) 0x080e0000)

// 获取指针偏移后的数值
#define FLASH_PTR_TO_UNSIGNED_CHAR(ptr, offset) ((unsigned char*)((uint32_t)ptr+offset))


// 区域1,用于存储配置数据偏移地址的偏移地址,这么写是为了加快读取速度,降低错误可能
unsigned int storageAddrOffset = 0;
// 区域2,数据存储区域偏移地址
unsigned int storageOffset = 0;

#define STORAGE_GET_OFFSET_U32() (*(uint32_t*)(ADDR_FLASH_SECTOR_10 + storageAddrOffset + 2))


void Flash_Clean(uint32_t Sector) {
    FLASH_Erase_Sector(Sector, FLASH_VOLTAGE_RANGE_3);
};

void Flash_WriteUInt8(uint32_t ptr, uint8_t word) {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ptr, word);
}

void Flash_WriteUInt32(uint32_t ptr, uint32_t word) {
    for (int i = 0; i < 4; ++i) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, ptr + i, ((unsigned char *) &word)[i]);
    }
}

// 存储数据区偏移地址
void Flash_SaveConfigShifting(uint32_t shifting) {
    storageOffset = shifting;
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    // 判断是否超出存储区域
    if (ADDR_FLASH_SECTOR_10 + storageAddrOffset + ((sizeof(shifting) + 2) * 2) >
        (ADDR_FLASH_SECTOR_10 + FLASH_SECTOR_SIZE)) {
        // 超出区域
        // 重新刷写
        Flash_Clean(10);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_10, 0x68);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_10 + 1, 0x79);
        Flash_WriteUInt32(ADDR_FLASH_SECTOR_10 + 2, shifting);
        storageAddrOffset = 0;
    } else {
        // 未超出区域
        // 写入数据
        auto lOffset = ADDR_FLASH_SECTOR_10 + storageAddrOffset + sizeof(shifting) + 2;
        Flash_WriteUInt8(lOffset, 0x68);
        Flash_WriteUInt8(lOffset + 1, 0x79);
        Flash_WriteUInt32(lOffset + 2, shifting);
        storageAddrOffset += sizeof(shifting) + 2;
    }
    HAL_FLASH_Lock();
}

void Flash_SaveConfig() {
    // 存储区标记地址
    if ((ADDR_FLASH_SECTOR_11 + storageOffset + ((sizeof(SysConfig) + 6) * 2)) >
        ADDR_FLASH_SECTOR_11 + FLASH_SECTOR_SIZE) {
        // 超出区域
        // 重新刷写
        // 重置偏移
        Flash_SaveConfigShifting(0);
        HAL_FLASH_Unlock();
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                               FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
        Flash_Clean(11);
        uint32_t cLen = sizeof(SysConfig);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11, ((unsigned char *) &cLen)[0]);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 1, ((unsigned char *) &cLen)[1]);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 2, ((unsigned char *) &cLen)[2]);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 3, ((unsigned char *) &cLen)[3]);
        for (int i = 0; i < sizeof(SysConfig); ++i) {
            Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 4 + i, ((unsigned char *) &g_SysConfig)[i]);
        }
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 4 + sizeof(SysConfig), 0x68);
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 4 + sizeof(SysConfig)+1, 0x79);
        HAL_FLASH_Lock();
    } else {
        Flash_SaveConfigShifting(storageOffset + sizeof(g_SysConfig) + 6);
        HAL_FLASH_Unlock();
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                               FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
        // 写入数据
        auto lOffset = ADDR_FLASH_SECTOR_11 + storageOffset;
        uint32_t cLen = sizeof(SysConfig);
        Flash_WriteUInt8(lOffset, ((unsigned char *) &cLen)[0]);
        Flash_WriteUInt8(lOffset + 1, ((unsigned char *) &cLen)[1]);
        Flash_WriteUInt8(lOffset + 2, ((unsigned char *) &cLen)[2]);
        Flash_WriteUInt8(lOffset + 3, ((unsigned char *) &cLen)[3]);
        //写配置
        for (int i = 0; i < sizeof(g_SysConfig); ++i) {
            Flash_WriteUInt8(lOffset + 4 + i, ((unsigned char *) &g_SysConfig)[i]);
        }
        Flash_WriteUInt8(lOffset + 4 + sizeof(SysConfig), 0x68);
        Flash_WriteUInt8(lOffset + 4 + sizeof(SysConfig)+1, 0x79);
        HAL_FLASH_Lock();
    }
}

void ResetStorage() {
    storageAddrOffset = 0;
    storageOffset = 0;
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    Flash_Clean(10);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_10, 0x68);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_10 + 1, 0x79);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_10 + 2, 0x0);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_10 + 3, 0x0);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_10 + 4, 0x0);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_10 + 5, 0x0);
    Flash_Clean(11);
    uint32_t cLen = sizeof(SysConfig);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_11, ((unsigned char *) &cLen)[0]);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 1, ((unsigned char *) &cLen)[1]);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 2, ((unsigned char *) &cLen)[2]);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 3, ((unsigned char *) &cLen)[3]);
    for (int i = 0; i < sizeof(SysConfig); ++i) {
        Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 4 + i, ((unsigned char *) &g_SysConfig)[i]);
    }
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 4 + sizeof(SysConfig), 0x68);
    Flash_WriteUInt8(ADDR_FLASH_SECTOR_11 + 4 + sizeof(SysConfig)+1, 0x79);
    HAL_FLASH_Lock();
}

void InitStorage() {
    // 读取存储区域首指针
    // 每次寻址6字节
    if (FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, 0)[0] != 0x68
        && FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, 0)[1] != 0x79) {
        // 没有初始化,需要重新刷写
        ResetStorage();
    }
    // 寻找偏移地址
    for (int i = 0; i < FLASH_SECTOR_SIZE; i += 6) {
        if (ADDR_FLASH_SECTOR_10 + i + 6 > ADDR_FLASH_SECTOR_10 + FLASH_SECTOR_SIZE) {
            // 超出区域
            break;
        }
        if (FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, i)[0] == 0x68
            && FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, i)[1] == 0x79) {
            // 读取偏移地址
            memcpy(&storageOffset, &FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, i)[2], sizeof(uint32_t));
            storageAddrOffset = i;
            continue;
        } else if (FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, i)[0] == 0xFF
                   && FLASH_PTR_TO_UNSIGNED_CHAR(ADDR_FLASH_SECTOR_10, i)[1] == 0xFF) {
            // 后端为未初始化
            break;
        }
    }
    // 读取配置
    if (ADDR_FLASH_SECTOR_11 + storageOffset + sizeof(SysConfig) + 6 > ADDR_FLASH_SECTOR_11 + FLASH_SECTOR_SIZE) {
        // 超出区域
        // 重新刷写
        ResetStorage();
    } else {
        // 读取配置
        auto scLen = *(uint32_t *) (ADDR_FLASH_SECTOR_11 + storageOffset);
        if (scLen != sizeof(SysConfig)) {
            // 配置长度错误
            Flash_SaveConfig();
        } else {
            auto checkPtr = ADDR_FLASH_SECTOR_11 + storageOffset + 4 + sizeof(SysConfig);
            auto a = FLASH_PTR_TO_UNSIGNED_CHAR(checkPtr, 0)[0];
            auto b = FLASH_PTR_TO_UNSIGNED_CHAR(checkPtr, 1)[0];
            if (FLASH_PTR_TO_UNSIGNED_CHAR(checkPtr, 0)[0] != 0x68
                || FLASH_PTR_TO_UNSIGNED_CHAR(checkPtr, 1)[0] != 0x79) {
                // 配置数据校验错误
                ResetStorage();
            } else {
                memcpy(&g_SysConfig, (void *) (ADDR_FLASH_SECTOR_11 + storageOffset + 4), sizeof(SysConfig));
            }
        }
    }
}

