//
// Created by 19254 on 2023/8/30.
//

#include "app_volume.hpp"

// 全局注册后只会调用一次,用于初始化,自行处理静态数据
void AppVolume::Init() {

};

// 进入事件
void AppVolume::In() {

};

// 刷新事件
void AppVolume::ReView() {
    OLED_CLEAR_BUFFER();
    drawA();
    OLED_SEND_BUFFER();
};

// 绘制界面(无HID版本)
void AppVolume::drawA() {

}

// 退出事件
void AppVolume::Out() {

};