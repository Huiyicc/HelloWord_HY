//
// Created by 19254 on 2023/9/4.
//
#include "usbproto.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"

namespace HYSDK::USB {

// wheel state:
// 1: 0x01 -> sroll up;
// 2: 0xff -> sroll down;
// 3: 0x80 -> do not thing;

void SendMouseUP() {
    uint8_t MouseBuffer[7] = {3, 0, 0, 0, 0, 0, 0x01};
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, MouseBuffer, 7);
}
void SendMouseDOWN() {
    uint8_t MouseBuffer[7] = {3,0, 0, 0, 0, 0, 0xff};
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, MouseBuffer, 7);
}

//void SendMouseLift() {
//    uint8_t MouseBuffer[7] = {3, 0, 0, 0, 0, 0, 0x01};
//    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, MouseBuffer, 7);
//}
//void SendMouseRight() {
//    uint8_t MouseBuffer[7] = {3,0, 0, 0, 0, 0, 0xff};
//    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, MouseBuffer, 7);
//}

void SendKeyBoardCode (int ctrl,KeyCode_t code) {
    uint8_t keyBoardData[9] = {2,0,0,0,0,0,0,0};
    uint8_t keyBoardDataEmpty[9] = {2,0,0,0,0,0,0,0};
    keyBoardData[1]=ctrl;
    keyBoardData[3]=code;
    HAL_Delay(2);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&keyBoardData,9);
    HAL_Delay(2);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&keyBoardDataEmpty,9);
}

}

