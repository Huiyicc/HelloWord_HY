//
// Created by 19254 on 2023/8/2.
//
#include "common_inc.h"
#include <map>
#include <string>
#include <queue.h>
#include <timers.h>
#include "ButtonPin.hpp"
#include "sleep.hpp"
#include "protocols/hid_msg.pb.h"
#include "pb_encode.h"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "SDK/usbproto.hpp"

ButtonPinCall ButtonPin;
//TimerHandle_t g_timer_button;

// 上一个按键状态
uint16_t lastButtonPin = 0;
// 长按监听
int longPressNum = 0;
// 是否已触发未弹起
bool isTrigger = false;

// 监听按钮任务
void ButtonTask(TimerHandle_t xTimer) {
    uint16_t buttonStatus = 0;
    if (HAL_GPIO_ReadPin(KEY_A_GPIO_Port, KEY_A_Pin) == GPIO_PIN_RESET) {
        if (HAL_GPIO_ReadPin(KEY_A_GPIO_Port, KEY_A_Pin) == GPIO_PIN_RESET) {
            buttonStatus = GPIO_PIN_8;
        }
    } else if (HAL_GPIO_ReadPin(KEY_B_GPIO_Port, KEY_B_Pin) == GPIO_PIN_RESET) {
        if (HAL_GPIO_ReadPin(KEY_B_GPIO_Port, KEY_B_Pin) == GPIO_PIN_RESET) {
            buttonStatus = GPIO_PIN_9;
        }
    }
    if (isTrigger) {
        // 已经触发了一个长按事件
        if (buttonStatus == 0) {
            // 释放了按键
            isTrigger = false;
            longPressNum = 0;
            lastButtonPin = 0;
            return;
        }
        return;
    }
    // 更换了按键状态
    if (buttonStatus != 0 && buttonStatus != lastButtonPin && lastButtonPin != 0) {
        // 重置
        longPressNum = 0;
        lastButtonPin = buttonStatus;
        return;
    } else if (buttonStatus == 0 && lastButtonPin != 0) {
        //更换按键状态且上一次不为0,本次释放,触发释放事件
        if (ButtonPin) {
            if (lastButtonPin == GPIO_PIN_8) {
                ButtonPin(ButtonPinCallType::LeftButtonPressed);
            } else if (lastButtonPin == GPIO_PIN_9) {
                ButtonPin(ButtonPinCallType::RightButtonPressed);
            }
        }
        // 重置
        longPressNum = 0;
        lastButtonPin = 0;
        return;
    }
    // 按键状态未改变
    if (buttonStatus == lastButtonPin && lastButtonPin != 0) {
        longPressNum++;
        if (longPressNum == 6) {
            // 长按事件
            if (buttonStatus == GPIO_PIN_8) {
                if (ButtonPin) {
                    ButtonPin(ButtonPinCallType::LeftButtonLongPress);
                }
            } else if (buttonStatus == GPIO_PIN_9) {
                if (ButtonPin) {
                    ButtonPin(ButtonPinCallType::RightButtonLongPress);
                }
            }
            isTrigger = true;
            return;
        }
    }
    lastButtonPin = buttonStatus;
}

// 注册按键回调
bool RegisterButtonPinCall(ButtonPinCall call) {
    if (!call) {
        return false;
    }
    g_sysCtx->ButtonPins.PushBack(call);
    return true;
}

// 按键回调
void privateCall(ButtonPinCallType type) {
    auto ptr = g_sysCtx->ButtonPins.GetHeadPtr();
    while (ptr) {
        if (ptr->val) {
            ptr->val(type);
        }
        ptr = ptr->pNext;
    }
}

void GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    //__HAL_RCC_GPIOC_CLK_ENABLE();

    /* Configure GPIO pins for Key_A and Key_B */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; //  input模式
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 上拉
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// 延迟休眠
void ButtonPinDelaySleep(ButtonPinCallType type) {
    OSDelaySleep();
}

// 上报按钮事件
void ButtonPinSendButton(ButtonPinCallType type) {
  uint8_t lBuffer[65] = {0};
  memset(lBuffer, 0, sizeof(lBuffer));
  lBuffer[0] = 0x06;
  lBuffer[1] = 0;
  pb_ostream_t stream = pb_ostream_from_buffer(&lBuffer[3], 62);
  hid_msg_CtrlPluginMessage msg = hid_msg_CtrlPluginMessage_init_default;
  msg.id = hid_msg_PluginMessageId_BUTTON_STATUS;
  hid_msg_ButtonStatus bs = hid_msg_ButtonStatus_init_default;
  bs.status = _hid_msg_ButtonPinCallType(type);
  msg.which_payload = hid_msg_CtrlPluginMessage_button_status_tag;
  msg.payload.button_status = bs;
  auto status = pb_encode(&stream, hid_msg_CtrlPluginMessage_fields, &msg);
  auto message_length = stream.bytes_written;
  if (message_length > 62 || !status) {
    // 数据错误
    lBuffer[2] = 0;
    HYSDK::USB::USBDHIDSendReport(&hUsbDeviceFS, lBuffer, 65, 3);
    return;
  }
  lBuffer[2] = message_length + 1;
  HYSDK::USB::USBDHIDSendReport(&hUsbDeviceFS, lBuffer, 65, 3);
}

// 初始化按键回调
void ButtonPinInit() {
    GPIO_Init();
    ButtonPin = privateCall;
    RegisterButtonPinCall(ButtonPinDelaySleep);
    RegisterButtonPinCall(ButtonPinSendButton);
    // 设置扫描定时器,200ms扫描一次
    //xTimerStart(xTimerCreate("ButtonTimer", pdMS_TO_TICKS(100), pdTRUE, nullptr, ButtonTask), 0);
}