//
// Created by 19254 on 2023/9/3.
//
#include "usbd.hpp"
#include "SDK/utils.hpp"
#include "usbd_conf.h"

unsigned char USB_Recive_Buffer[65]; //USB接收缓存

void Usb_DataEvent() {
    //USBD_DbgLog("1232");
    Println("%s",USB_Recive_Buffer);
    memset(USB_Recive_Buffer, 0, sizeof(USB_Recive_Buffer));

}USBDataEventFunc USBDataEvent = Usb_DataEvent;