//
// Created by 19254 on 2023/9/3.
//
#include "freertos_os2.h"
#include "usbd.hpp"
#include "SDK/utils.hpp"
#include "usbd_conf.h"
#include "SDK/git.hpp"
#include "protocols/pb_decode.h"
#include "protocols/pb_encode.h"
#include "protocols/hid_msg.pb.h"
#include "usbd_customhid.h"
#include "usb_device.h"
#include "cmsis_os.h"
#include "os_define.hpp"
#include "eink_290_bw.h"
#include "app_desktop.hpp"

unsigned char USB_Recive_Buffer[65]; //USB接收缓存
unsigned char USB_Recive_Tmp_Buffer[5000]; //USB发送缓存
unsigned int rec_offset = 0; //EINK偏移量
unsigned char eink_status = 0;

bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
    char *_str = (char *) *arg;
    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t *) _str, strlen(_str));
}


void HID_SendVersion() {
    uint8_t lBuffer[65] = {0};
    memset(lBuffer, 0, sizeof(lBuffer));
    lBuffer[0] = 0x04;
    lBuffer[1] = 0;
    pb_ostream_t stream = pb_ostream_from_buffer(&lBuffer[3], 62);
    hid_msg_CtrlMessage msg = hid_msg_CtrlMessage_init_default;
    msg.id = hid_msg_MessageId_VERSION;
    hid_msg_Version version = hid_msg_Version_init_default;
    version.GitVer.funcs.encode = encode_string;
    version.GitVer.arg = (void *) GIT_VERSION;
    version.GitBranch.funcs.encode = encode_string;
    version.GitBranch.arg = (void *) GIT_BRANCH;
    version.GitHash.funcs.encode = encode_string;
    version.GitHash.arg = (void *) GIT_HASH;
    msg.which_payload = hid_msg_CtrlMessage_version_tag;
    msg.payload.version = version;
    auto status = pb_encode(&stream, hid_msg_CtrlMessage_fields, &msg);
    auto message_length = stream.bytes_written;
    if (message_length > 62 || !status) {
        // 数据错误
        lBuffer[2] = 0;
        USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
        osDelay(3);
        return;
    }
    lBuffer[2] = message_length + 1;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
    osDelay(3);
}


void Usb_DataEvent() {
    // 通信帧首字节为报告id,第二字节为完整性(0为完整,1为单次不够放入),第三字节为数据长度
    if (USB_Recive_Buffer[0] != 0x04) {
        // 无效的报告ID
        memset(USB_Recive_Buffer, 0, sizeof(USB_Recive_Buffer));
        return;
    }
    // 数据处理
    if (USB_Recive_Buffer[2] > 64) {
        // 数据长度错误
        memset(USB_Recive_Buffer, 0, sizeof(USB_Recive_Buffer));
        return;
    }
    if (USB_Recive_Buffer[1] == 1) {
        // 拼接数据
        memcpy(USB_Recive_Tmp_Buffer + rec_offset, USB_Recive_Buffer + 3, USB_Recive_Buffer[2] - 1);
        rec_offset += USB_Recive_Buffer[2] - 1;
        eink_status = 0;
        return;
    }
    if (USB_Recive_Buffer[1] == 2) {
        // 图像模式
        // 拼接数据
        memcpy(USB_Recive_Tmp_Buffer + rec_offset, USB_Recive_Buffer + 3, USB_Recive_Buffer[2] - 1);
        rec_offset += USB_Recive_Buffer[2] - 1;
        eink_status = 1;
        return;
    }
    if (USB_Recive_Buffer[1] == 0 && rec_offset != 0) {
        // 上一次数据未处理完
        memcpy(USB_Recive_Tmp_Buffer + rec_offset, USB_Recive_Buffer + 3, USB_Recive_Buffer[2] - 1);
        rec_offset += USB_Recive_Buffer[2] - 1;
    }
    // 初始化解包上下文
    pb_istream_t stream;
    //stream.callback = de_callback;
    if (rec_offset == 0) {
        stream = pb_istream_from_buffer(&USB_Recive_Buffer[3], size_t(USB_Recive_Buffer[2] - 1));
        eink_status = 0;
    } else if (rec_offset != 0 && USB_Recive_Buffer[1] == 0 && eink_status != 1) {
        stream = pb_istream_from_buffer(USB_Recive_Tmp_Buffer, size_t(rec_offset));
        rec_offset = 0;
    } else if (eink_status == 1) {
        eink_status = 0;
        int maxsize = (EPD_HEIGHT * EPD_WIDTH / 8);
        if (rec_offset != maxsize) {
            return;
        }
        g_sysCtx->Device.eink->Init();
        g_sysCtx->Device.eink->DrawBitmap(USB_Recive_Tmp_Buffer);
        g_sysCtx->Device.eink->Update();
        g_sysCtx->Device.eink->DeepSleep();
        return;
    } else {
        return;
    }

    hid_msg_PcMessage msg;
    pb_decode(&stream, hid_msg_PcMessage_fields, &msg);
    switch (msg.id) {
        case hid_msg_MessageId_VERSION: {
            //回报版本号
            HID_SendVersion();
        }
            break;
        case hid_msg_MessageId_MOTOR_SET_CONFIG: {
            switch (msg.payload.knob.id) {
                default:
                    break;
                case hid_msg_knobMessage_SetPID: {
                    g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t(msg.payload.knob.knobModel));
                    g_sysCtx->Device.ctrl.knob.SetAnglePID(msg.payload.knob.payload.pid.angle.p,
                                                           msg.payload.knob.payload.pid.angle.i,
                                                           msg.payload.knob.payload.pid.angle.d);
                    g_sysCtx->Device.ctrl.knob.SetVelocityPID(msg.payload.knob.payload.pid.velocity.p,
                                                              msg.payload.knob.payload.pid.velocity.i,
                                                              msg.payload.knob.payload.pid.velocity.d);
                }
                    break;
            }

        }
            break;
        case hid_msg_MessageId_DEV_UTILS:
            // 1：设置菜单动画
            if (msg.payload.utils.id == 1) {
                auto app = (AppDesktop *) g_sysCtx->Apps.AppsMap[APPID_DESKTOP];
                app->EasingType = msg.payload.utils.uData;
            }
        default:
            memset(USB_Recive_Buffer, 0, sizeof(USB_Recive_Buffer));
            break;
    }


}

USBDataEventFunc USBDataEvent = Usb_DataEvent;