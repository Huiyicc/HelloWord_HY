//
// Created by 19254 on 2023/9/3.
//
#include "utils.hpp"

#include "usb_device.h"
#include "usbd_customhid.h"

namespace HYSDK::Debug {
void Print(const char *str) {
    uint8_t HID_report[64] = {' '};
    memcpy(HID_report, str, strlen(str));
    HID_report[63] = '\n';
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_report, 64);
}
}