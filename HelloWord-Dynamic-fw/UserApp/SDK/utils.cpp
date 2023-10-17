//
// Created by 19254 on 2023/9/3.
//
#include "utils.hpp"

#include "usb_device.h"
#include "usbd_customhid.h"
#include "cmsis_os.h"

bool debug = true;

namespace HYSDK::Debug {

void Print(unsigned char reportID,const char *str) {
    if (!debug) {
        return;
    }
    uint8_t HID_report[65] = {'a'};
    memcpy(HID_report+1, str, strlen(str));
    HID_report[0] = reportID;
    HID_report[64] = '\n';
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_report, 65);
    osDelay(3);
}


}