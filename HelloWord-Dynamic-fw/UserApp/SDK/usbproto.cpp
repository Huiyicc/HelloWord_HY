//
// Created by 19254 on 2023/9/4.
//
#include "usbproto.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "cmsis_os2.h"
#include "mutex"
#include <atomic>

namespace HYSDK::USB {

std::atomic<int> hidSendLock(0);

uint8_t USBDHIDSendReport(void *pdev,
                          uint8_t *report, uint16_t len, uint8_t delay) {
  if (delay > 0) {
    osDelay(delay);
  }
  int expected = hidSendLock.load(std::memory_order_relaxed);
  while (!hidSendLock.compare_exchange_weak(expected, 1, std::memory_order_acquire, std::memory_order_relaxed)) {
    expected = 0; // 如果锁被其他线程持有，则重新加载预期值
  }
  auto result = USBD_CUSTOM_HID_SendReport((USBD_HandleTypeDef *) pdev, report, len);
  // 完成
  hidSendLock.store(0, std::memory_order_release);
  return result;
}

// wheel state:
// 1: 0x01 -> sroll up;
// 2: 0xff -> sroll down;
// 3: 0x80 -> do not thing;

void SendMouseUP(int num) {
  char n = 0x01 + num - 1;
  uint8_t MouseBuffer[7] = {3, 0, 0, 0, 0, 0, n};
  USBDHIDSendReport(&hUsbDeviceFS, MouseBuffer, 7,1);
  //osDelay(1);
}

void SendMouseDOWN(int num) {
  char n = 0xff - num + 1;
  uint8_t MouseBuffer[7] = {3, 0, 0, 0, 0, 0, n};
  USBDHIDSendReport(&hUsbDeviceFS, MouseBuffer, 7,1);
  //osDelay(1);
}

void SendKeyBoardCode(int ctrl, KeyCode_t code) {
  uint8_t keyBoardData[9] = {2, 0, 0, 0, 0, 0, 0, 0};
  uint8_t keyBoardDataEmpty[9] = {2, 0, 0, 0, 0, 0, 0, 0};
  keyBoardData[1] = ctrl;
  keyBoardData[3] = code;
  // HAL_Delay(2);
  USBDHIDSendReport(&hUsbDeviceFS, (uint8_t *) &keyBoardData, 9,2);
  //HAL_Delay(2);
  USBDHIDSendReport(&hUsbDeviceFS, (uint8_t *) &keyBoardDataEmpty, 9,2);
}

}

