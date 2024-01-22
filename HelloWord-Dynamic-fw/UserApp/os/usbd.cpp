//
// Created by 19254 on 2023/9/3.
//
#include "freertos_os2.h"
#include "usbd.hpp"
#include "SDK/utils.hpp"
#include "protocols/pb_decode.h"
#include "protocols/pb_encode.h"
#include "protocols/hid_msg.pb.h"
#include "usbd_customhid.h"
#include "usb_device.h"
#include "cmsis_os.h"
#include "os_define.hpp"
//#include "eink_290_bw.h"
#include "app_desktop.hpp"
#include "storage.hpp"

unsigned char USB_Recive_Buffer[65]; //USB接收缓存
unsigned char USB_Recive_Tmp_Buffer[5000]; //USB发送缓存
unsigned int rec_offset = 0; //EINK偏移量
unsigned char eink_status = 0;
unsigned char oled_status = 0;
extern bool hidApp;

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
  version.BuildVersion.funcs.encode = encode_string;
  version.BuildVersion.arg = (void *) BUILD_VERSION;
  msg.which_payload = hid_msg_CtrlMessage_version_tag;
  msg.payload.version = version;
  auto status = pb_encode(&stream, hid_msg_CtrlMessage_fields, &msg);
  auto message_length = stream.bytes_written;
  if (message_length > 62 || !status) {
    // 数据错误
    lBuffer[2] = 0;
    osDelay(3);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
    return;
  }
  lBuffer[2] = message_length + 1;
  osDelay(3);
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
}

void HID_SendAppConf(uint32_t appid) {
  uint8_t lBuffer[65] = {0};
  memset(lBuffer, 0, sizeof(lBuffer));
  lBuffer[0] = 0x04;
  lBuffer[1] = 0;
  pb_ostream_t stream = pb_ostream_from_buffer(&lBuffer[3], 62);
  hid_msg_CtrlMessage msg = hid_msg_CtrlMessage_init_default;
  msg.id = hid_msg_MessageId_MOTOR_CONFIG;
  hid_msg_Knob knob = hid_msg_Knob_init_default;
  knob.id = hid_msg_KnobMessage_GetAppConfig;
  knob.appid = appid;
  bool check = true;
  AppKnobConfig *appsConfig = nullptr;
  switch (appid) {
    case APPID_VOLUME:
      appsConfig = &g_SysConfig.apps.Volume;
      break;

    case APPID_UPDOWN:
      appsConfig = &g_SysConfig.apps.UpDown;
      break;

    case APPID_LIGHT:
      appsConfig = &g_SysConfig.apps.Light;
      break;

    case APPID_LEFTRIGHT:
      appsConfig = &g_SysConfig.apps.LeftRight;
      break;

    default:
      check = false;
      break;
  }
  knob.knobMode = appsConfig->Mode;
  knob.stepConf.value = appsConfig->EncodePos;
  knob.torqueLimitConf.value = appsConfig->TorqueLimit;
  knob.velocityLimitConf.value = appsConfig->VelocityLimit;
  knob.AddedValue = appsConfig->AddedValue;
//  knob.stepConf.max = 36;
//  knob.stepConf.min = 5;
//  knob.torqueLimitConf.max = 3.5f;
//  knob.torqueLimitConf.min = 0.1f;
//  knob.velocityLimitConf.min = 0.4f;
//  knob.velocityLimitConf.max = 5;

  if (!check) {
    // appid不存在
    return;
  }
  msg.which_payload = hid_msg_CtrlMessage_knob_tag;
  msg.payload.knob = knob;
  auto status = pb_encode(&stream, hid_msg_CtrlMessage_fields, &msg);
  auto message_length = stream.bytes_written;
  if (message_length > 62 || !status) {
    // 数据错误
    lBuffer[2] = 0;
    osDelay(3);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
    return;
  }
  lBuffer[2] = message_length + 1;
  osDelay(3);
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);

}

// 设置APP配置
void HID_SetAppConf(const _hid_msg_Knob *conf) {
  uint8_t lBuffer[65] = {0};
  memset(lBuffer, 0, sizeof(lBuffer));
  lBuffer[0] = 0x04;
  AppKnobConfig *appConf = nullptr;
  switch (conf->appid) {
    case APPID_VOLUME:
      appConf = &g_SysConfig.apps.Volume;
      break;

    case APPID_UPDOWN:
      appConf = &g_SysConfig.apps.UpDown;
      break;

    case APPID_LIGHT:
      appConf = &g_SysConfig.apps.Light;
      break;

    case APPID_LEFTRIGHT:
      appConf = &g_SysConfig.apps.LeftRight;
      break;

    default:
      osDelay(2);
      USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
      return;
  }

  switch (conf->setAppType) {
    case _hid_msg_SetAppType::hid_msg_SetAppType_KnobMode: {
      appConf->Mode = conf->knobMode;
      if (g_sysCtx->Apps.Status == conf->appid) {
        g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t(conf->knobMode), appConf);
        g_sysCtx->Device.ctrl.knob.UpdateConf(appConf);
      }
    }
      break;
    case _hid_msg_SetAppType::hid_msg_SetAppType_Step: {
      appConf->EncodePos = conf->stepConf.value;
      if (g_sysCtx->Apps.Status == conf->setAppType) {
        g_sysCtx->Device.ctrl.knob.SetEncoderModePos(appConf->EncodePos);
      }
    }
      break;
    case _hid_msg_SetAppType::hid_msg_SetAppType_AddedValue: {
      appConf->AddedValue = conf->AddedValue;
    }
      break;
    case _hid_msg_SetAppType::hid_msg_SetAppType_TorqueLimit: {
      appConf->TorqueLimit = conf->torqueLimitConf.value;
      if (g_sysCtx->Apps.Status == conf->appid) {
        g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t(conf->knobMode), appConf);
        g_sysCtx->Device.ctrl.knob.UpdateConf(appConf);
      }
    }
      break;
    case _hid_msg_SetAppType::hid_msg_SetAppType_VelocityLimit: {
      appConf->VelocityLimit = conf->velocityLimitConf.value;
      g_sysCtx->Device.ctrl.knob.triggerVelocityMax = appConf->VelocityLimit;
    }
      break;
    default:
      osDelay(2);
      USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
      return;
  }
  GetSysConfig(true);
  // 成功标志
  lBuffer[1] = 0x01;
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);

}

// 发送系统配置信息
void HID_SendSysConf() {
  uint8_t lBuffer[65] = {0};
  memset(lBuffer, 0, sizeof(lBuffer));
  lBuffer[0] = 0x04;
  lBuffer[1] = 0;
  pb_ostream_t stream = pb_ostream_from_buffer(&lBuffer[3], 62);
  hid_msg_CtrlMessage msg = hid_msg_CtrlMessage_init_default;
  msg.id = hid_msg_MessageId_GET_SYS_CFG;
  hid_msg_SysCfg conf = hid_msg_SysCfg_init_default;
  conf.SleepTime = g_SysConfig.sys.SleepTime;
  msg.which_payload = hid_msg_CtrlMessage_sys_cfg_tag;
  msg.payload.sys_cfg = conf;
  auto status = pb_encode(&stream, hid_msg_CtrlMessage_fields, &msg);
  auto message_length = stream.bytes_written;
  if (message_length > 62 || !status) {
    // 数据错误
    lBuffer[2] = 0;
    osDelay(3);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
    return;
  }
  lBuffer[2] = message_length + 1;
  osDelay(3);
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
}

// 设置APP配置
void HID_SetSysCong(const _hid_msg_SysCfg *conf) {
  uint8_t lBuffer[65] = {0};
  memset(lBuffer, 0, sizeof(lBuffer));
  lBuffer[0] = 0x04;
  AppKnobConfig *appConf = nullptr;
  switch (conf->type) {
    case hid_msg_SetSysCfgType_SleepTime: {
      auto sysCfg = GetSysConfig();
      sysCfg->sys.SleepTime = conf->SleepTime;
    }
      break;
    default: {
      osDelay(2);
      USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
      return;
    }
  }
  GetSysConfig(true);
  // 成功标志
  lBuffer[1] = 0x01;
  osDelay(2);
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
}

extern KnobStatus *knobStatus;

void HID_GetKnobStatus() {
  uint8_t lBuffer[65] = {0};
  memset(lBuffer, 0, sizeof(lBuffer));
  lBuffer[0] = 0x04;
  lBuffer[1] = 0;
  pb_ostream_t stream = pb_ostream_from_buffer(&lBuffer[3], 62);
  hid_msg_CtrlMessage msg = hid_msg_CtrlMessage_init_default;
  msg.id = hid_msg_MessageId_MOTOR_GET_STATUS;
  hid_msg_knobStatus conf = hid_msg_knobStatus_init_default;
  conf.timestamp = HAL_GetTick();
  conf.knobMode = g_sysCtx->Device.ctrl.knob.GetMode();
  conf.current_angle = knobStatus->Angle;
  conf.target_angle = g_sysCtx->Device.ctrl.knob.GetTarget();
  conf.current_velocity = knobStatus->Velocity;
  conf.target_velocity = g_sysCtx->Device.ctrl.knob.GetTarget();
  conf.target_voltage = sqrt(g_sysCtx->Device.ctrl.motor.voltage.q * g_sysCtx->Device.ctrl.motor.voltage.q +
                             g_sysCtx->Device.ctrl.motor.voltage.d * g_sysCtx->Device.ctrl.motor.voltage.d);
  msg.which_payload = hid_msg_CtrlMessage_knob_status_tag;
  msg.payload.knob_status = conf;
  auto status = pb_encode(&stream, hid_msg_CtrlMessage_fields, &msg);
  auto message_length = stream.bytes_written;
  if (message_length > 62 || !status) {
    // 数据错误
    lBuffer[2] = 0;
    osDelay(3);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
    return;
  }
  lBuffer[2] = message_length + 1;
  osDelay(3);
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, 65);
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
  } else if (USB_Recive_Buffer[1] == 2) {
    // 图像模式
    // 拼接数据
    memcpy(USB_Recive_Tmp_Buffer + rec_offset, USB_Recive_Buffer + 3, USB_Recive_Buffer[2] - 1);
    rec_offset += USB_Recive_Buffer[2] - 1;
    eink_status = 1;
    return;
  } else if (USB_Recive_Buffer[1] == 3) {
    // OLED模式
    // 拼接数据
    memcpy(USB_Recive_Tmp_Buffer + rec_offset, USB_Recive_Buffer + 3, USB_Recive_Buffer[2] - 1);
    rec_offset += USB_Recive_Buffer[2] - 1;
    oled_status = 1;
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
    oled_status = 0;
  } else if (rec_offset != 0 && USB_Recive_Buffer[1] == 0 && eink_status != 1 && oled_status != 1) {
    stream = pb_istream_from_buffer(USB_Recive_Tmp_Buffer, size_t(rec_offset));
    rec_offset = 0;
  } else if (eink_status == 1) {
    eink_status = 0;
    int maxsize = (EPD_HEIGHT * EPD_WIDTH / 8);
    if (rec_offset != maxsize) {
      rec_offset = 0;
      return;
    }
    rec_offset = 0;
    g_sysCtx->Device.eink->DrawBitmap(USB_Recive_Tmp_Buffer);
    g_sysCtx->Device.eink->Update();
    uint8_t lBuffer[65] = {0};
    memset(lBuffer, 0, sizeof(lBuffer));
    lBuffer[0] = 0x04;
    lBuffer[1] = 0x02;
    lBuffer[2] = 0x02;
    lBuffer[3] = 0x02;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, sizeof(lBuffer));
    return;
  } else if (oled_status == 1) {
    if (!hidApp) {
      return;
    }
    oled_status = 0;
    rec_offset = 0;
    // 32*128 = 4096
    // 4096/8 = 512
    OLED_CLEAR_BUFFER();
    OLED_DEVICES()->DrawXBM(0, 0, 32, 128, USB_Recive_Tmp_Buffer);
    OLED_SEND_BUFFER();
    uint8_t lBuffer[65] = {0};
    memset(lBuffer, 0, sizeof(lBuffer));
    lBuffer[0] = 0x04;
    lBuffer[1] = 0x02;
    lBuffer[2] = 0x02;
    lBuffer[3] = 0x02;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, lBuffer, sizeof(lBuffer));
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
    case hid_msg_MessageId_MOTOR_CONFIG : {
      switch (msg.payload.knob.id) {
        case hid_msg_KnobMessage_GetAppConfig:
          // 获取APP配置
          HID_SendAppConf(msg.payload.knob.appid);
          break;
        case hid_msg_KnobMessage_SetAppConfig:
          // 设置APP配置
          HID_SetAppConf(&msg.payload.knob);
          break;
      }
    }
      break;
    case hid_msg_MessageId_GET_SYS_CFG: {
      HID_SendSysConf();
    }
      break;
    case hid_msg_MessageId_SET_SYS_CFG: {
      // 设置系统配置
      HID_SetSysCong(&msg.payload.sys_cfg);
    }
      break;
    case hid_msg_MessageId_DEV_UTILS: {
      // 1：设置菜单动画
      if (msg.payload.utils.id == 1) {
        auto app = (AppDesktop *) g_sysCtx->Apps.AppsMap[APPID_DESKTOP];
        app->EasingType = msg.payload.utils.uData;
      }
    }
      break;
    case hid_msg_MessageId_MOTOR_GET_STATUS: {
      HID_GetKnobStatus();
    }
      break;
    default:
      memset(USB_Recive_Buffer, 0, sizeof(USB_Recive_Buffer));
      break;
  }


}

USBDataEventFunc USBDataEvent = Usb_DataEvent;