//
// Created by 19254 on 2023/9/3.
//

#ifndef HELLOWORD_DYNAMIC_FW_USBD_HPP
#define HELLOWORD_DYNAMIC_FW_USBD_HPP

typedef void(*USBDataEventFunc)();
extern unsigned char USB_Recive_Buffer[65]; //USB接收缓存
extern USBDataEventFunc USBDataEvent;


#endif //HELLOWORD_DYNAMIC_FW_USBD_HPP
