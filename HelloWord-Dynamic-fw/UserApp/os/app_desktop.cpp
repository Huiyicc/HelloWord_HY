//
// Created by 19254 on 2023/8/27.
//
#include "app_desktop.hpp"
#include "SDK/Easingcpp.hpp"
#include "SDK/math.hpp"
#include "SDK/utils.hpp"
#include "app_leftright.hpp"
#include "ctrl.hpp"
#include "os/ButtonPin.hpp"

void appDesktopButtonPinCallback(enum ButtonPinCallType type) {
  if (g_sysCtx->Apps.Status != APPID_DESKTOP) {
    return;
  }
  //// 下辈子在写这种代码老子就是狗（焯
  //auto info = ((AppDesktop *) &(g_sysCtx->Apps.Apps[APPID_DESKTOP]))->GetDesktopInfo();
  auto info = ((AppDesktop *) (g_sysCtx->Apps.AppsMap[APPID_DESKTOP]))->GetDesktopInfo();
  switch (type) {
    case ButtonPinCallType::RightButtonPressed:
      AppChange(info->AppID);
      break;
    default:
      break;
  }
}

void EasingCallBack(
    double schedule,//当前进度
    double cValue,  //当前缓动值
    int totalframes,//总帧数
    int nframe,     //当前帧数
    bool &stop,     //是否停止
    int param1,
    void *param2) {
  auto _this = (AppDesktop *) param2;
  if (param1 == 0) {
    _this->SetShifting(float(0 - 43) + float(43) * cValue);
  } else {
    _this->SetShifting(float(0) - float(43) * cValue);
  }
  _this->ReView();
}

void KNobCallback(bool move, KnobStatus *status) {
  if (g_sysCtx->Apps.Status != APPID_DESKTOP) {
    return;
  }
  auto _this = ((AppDesktop *) (g_sysCtx->Apps.AppsMap[APPID_DESKTOP]));
  auto la = g_sysCtx->Device.ctrl.knob.lastAngle + g_sysCtx->Device.ctrl.knob.deviation;
  int targetEncoderPosition;
  if (status->Position > la) {
    targetEncoderPosition = status->EncoderPosition + 1;
  } else if (status->Position < la) {
    targetEncoderPosition = status->EncoderPosition - 1;
  } else {
    targetEncoderPosition = status->EncoderPosition;
  }
  _this->ReViewToMove(move, status, targetEncoderPosition);
  //  if (status->EncoderPosition != status->LastEncoderPosition) {
  //    if (status->EncoderPosition > status->LastEncoderPosition) {
  //      _this->MenuDec();
  //    } else {
  //      _this->MenuAdd();
  //    }
  //  }
}

//初始化事件
void AppDesktop::Init() {
  // 不要小于3个,虽然做了处理,但是没测试,会有bug
  m_menuNum = 6;
  char i = 0;
  m_menuList = (desktopCallType *) pvPortMalloc(sizeof(desktopCallType) * m_menuNum);
  m_menuList[i] = desktopCallType{"音量",
                                  "0",
                                  APPID_VOLUME};
  i++;
  m_menuList[i] = desktopCallType{"滚动",
                                  "3",
                                  APPID_UPDOWN};
  i++;
  m_menuList[i] = desktopCallType{"亮度",
                                  "1",
                                  APPID_LIGHT};
  i++;
  m_menuList[i] = desktopCallType{"视窗",
                                  "4",
                                  APPID_WINDOW};
  i++;
  m_menuList[i] = desktopCallType{"滑动",
                                  "2",
                                  APPID_LEFTRIGHT};
  i++;
  m_menuList[i] = desktopCallType{"APP",
                                  "5",
                                  APPID_HID};
  i++;
  RegisterKNobCallback(KNobCallback);
  RegisterButtonPinCall(appDesktopButtonPinCallback);
}

// 进入事件
void AppDesktop::In() {
  g_sysCtx->Device.ctrl.knob.SetEncoderModePos(int(m_dividesNum));
  g_sysCtx->Device.ctrl.knob.SetMode(KnobSimulator::Mode_t::MODE_JINLUNENCODER, nullptr);
  g_sysCtx->Device.ctrl.knob.SetTorqueLimit(0.5);
  ReView();
}

// 标题高度
constexpr int MenuTopHeight = 25,
              // 标题间隔
    MenuTopInterval = 10,
              // 图标高度
    MenuIconHeight = 35,
              // 标记位置
    MarkPosition = MenuTopHeight - MenuTopInterval + (MenuIconHeight + (MenuIconHeight / 2)) + 2,
              // 标记移动长度
    MarkShiftingHeight = MenuTopHeight;

// 刷新事件
void AppDesktop::ReView() {
  OLED_CLEAR_BUFFER();
  int y;
  //OLED_DEVICES()->SetDrawColor(1);
  //OLED_DEVICES()->DrawBox(0, 58, 32, 32);
  y = MenuTopHeight;

  OLED_DEVICES()->SetDrawColor(1);
  OLED_DEVICES()->SetFont(font_default);
  OLED_DEVICES()->DrawUTF8(5, 0, m_menuList[m_menuIndex].name);

  OLED_DEVICES()->SetDrawColor(2);
  OLED_DEVICES()->SetFont(hwd_hy_U24);
  unsigned char maxs = 0;
  auto arrs = getIndexArr(&maxs);
  for (unsigned i = 0; i < maxs; i++) {
    OLED_DEVICES()->DrawStr(4, y, m_menuList[arrs[i]].icon);
    y += MenuIconHeight;
  }
  vPortFree(arrs);
  OLED_DEVICES()->SetDrawColor(1);
  OLED_DEVICES()->DrawRBox(0, MarkPosition , 3, 4 , 1);
  OLED_DEVICES()->DrawRBox(29, MarkPosition , 3, 4 , 1);
  OLED_SEND_BUFFER();
}

void AppDesktop::ReViewToMove(bool move, KnobStatus *status, float_t targetEncoderPosition) {
  float mpShifting = 0;
  bool mpType = false;
  m_shifting = 0;
  if (status->EncoderPosition != status->LastEncoderPosition) {
    if (status->EncoderPosition > status->LastEncoderPosition) {
      // add
      m_menuIndex = m_menuIndex == m_menuNum - 1 ? 0 : m_menuIndex + 1;
    } else {
      // dec
      m_menuIndex = m_menuIndex == 0 ? m_menuNum - 1 : m_menuIndex - 1;
    }
  }
  unsigned char maxs = 0;
  unsigned char *arrs;
  int y = MenuTopHeight;
  if (status->EncoderPosition != targetEncoderPosition) {
    float targetPosition = g_sysCtx->Device.ctrl.knob.GetPositionFromEncoderModePos(targetEncoderPosition) + g_sysCtx->Device.ctrl.knob.deviation;
    if (status->Position > targetPosition) {
      // ↓
      m_status = 1;
      arrs = getIndexArr(&maxs);
      float diff = std::abs(status->Position - targetPosition);
      float bd = diff / g_sysCtx->Device.ctrl.knob.encoderDistance;
      bd = _EasingType_CircularCurve_Out(bd, 0, 1);
      m_shifting = float(MenuTopHeight + MenuTopInterval) - float(MenuTopHeight + MenuTopInterval) * bd - MenuIconHeight;
      float dis = g_sysCtx->Device.ctrl.knob.encoderDistance * 0.4;
      if (diff > dis) {
        // 前半段拉长
        bd = (diff-dis) / dis;
        bd = _EasingType_IndexCurve_In(bd, 0, 1);
        mpShifting = (MarkShiftingHeight - MarkShiftingHeight * bd)/2;
        mpType = false;
      } else {
        // 后半段收回
        bd = diff / dis;
        bd = _EasingType_IndexCurve_Out(bd, 0, 1);
        mpShifting = (MarkShiftingHeight * bd)/2;
        mpType = false;

      }

    } else {
      // ↑
      m_status = 2;
      arrs = getIndexArr(&maxs);
      float diff = std::abs(targetPosition - status->Position);
      float bd = diff / g_sysCtx->Device.ctrl.knob.encoderDistance;
      bd = _EasingType_CircularCurve_Out(bd, 0, 1);
      m_shifting = float(0 - MenuTopHeight - MenuTopInterval) + float(MenuIconHeight) * bd;
      float dis = g_sysCtx->Device.ctrl.knob.encoderDistance * 0.4;
      if (diff > dis) {
        // 前半段拉长
        bd = (diff - dis) / dis;
        bd = _EasingType_IndexCurve_In(bd, 0, 1);
        mpShifting = (MarkShiftingHeight - MarkShiftingHeight * bd)/2;
        mpType = true;
      } else {
        // 后半段收回
        bd = diff / dis;
        bd = _EasingType_IndexCurve_Out(bd, 0, 1);
        mpShifting = (MarkShiftingHeight * bd)/2;
        mpType = true;
      }
      //Println("↑");
    }

  } else {
    m_status = 0;
    arrs = getIndexArr(&maxs);
  }

  OLED_CLEAR_BUFFER();

  OLED_DEVICES()->SetDrawColor(2);
  OLED_DEVICES()->SetFont(hwd_hy_U24);
  y += m_shifting;

  for (unsigned i = 0; i < maxs; i++) {
    if (i == 0) {
      OLED_DEVICES()->SetDrawColor(1);
    } else {
      OLED_DEVICES()->SetDrawColor(2);
    }
    OLED_DEVICES()->DrawStr(4, y, m_menuList[arrs[i]].icon);
    if (i == 0) {
      OLED_DEVICES()->SetDrawColor(0);
      OLED_DEVICES()->DrawBox(0, 0, 32, 20);
    }
    y += MenuIconHeight;
  }
  OLED_DEVICES()->SetDrawColor(1);
  OLED_DEVICES()->SetFont(font_default);
  OLED_DEVICES()->DrawUTF8(5, 0, m_menuList[m_menuIndex].name);
  if (mpType) {
    // ↑
    OLED_DEVICES()->DrawRBox(0, MarkPosition + mpShifting, 3, 4 + mpShifting, 1);
    OLED_DEVICES()->DrawRBox(29, MarkPosition + mpShifting, 3, 4 + mpShifting, 1);
  } else {
    OLED_DEVICES()->DrawRBox(0, MarkPosition - (mpShifting *2), 3, 4 + mpShifting, 1);
    OLED_DEVICES()->DrawRBox(29, MarkPosition - (mpShifting *2), 3, 4 + mpShifting, 1);
  }

  vPortFree(arrs);
  m_status = 0;
  // OLED_DEVICES()->SetFont(u8g2_font_wqy12_t_gb2312);
  //  static int c = 8 , x = 55 , y = 52, r = 50;
  //
  //  auto n = float(m_menuNum+c) / _2PI;
  //  for (uint8_t i = 0; i < m_menuNum; i++) {
  //    auto point = HYSDK::calculateCirclePoint(x,62,r,n*float(i)+_PI+status->Position);
  //    OLED_DEVICES()->DrawStr(point.x, point.y, m_menuList[i].icon);
  //    Println("x:%f y:%f i:%s n:%f",point.x,point.y,m_menuList[i].icon,n);
  //  }
  OLED_SEND_BUFFER();

  //  m_status = 0;
  //  m_shifting = 0;
  //  if (status->EncoderPosition != targetEncoderPosition) {
  //    auto pro = (status->Position - g_sysCtx->Device.ctrl.knob.deviation - g_sysCtx->Device.ctrl.knob.lastAngle) /
  //               g_sysCtx->Device.ctrl.knob.encoderDistance;
  //    auto cValue = _EasingType_CircularCurve_Out(std::fabs(pro), 0, 1);
  //    if (status->EncoderPosition > targetEncoderPosition) {
  //      m_shifting = float(0 - MenuIconHeight) + float(MenuIconHeight) * cValue;
  //    } else {
  //      m_shifting = float(0) - float(MenuIconHeight) * cValue;
  //    }
  //
  //  }
  //  ReView();
}

// 退出事件
void AppDesktop::Out() {
}

//unsigned char *AppDesktop::getIndexArr(unsigned char *size) {
//  auto arr = (unsigned char *) pvPortMalloc(sizeof(unsigned char) * 4);
//  memset(arr, 0, sizeof(unsigned char) * 4);
//  // 不足4个
//  if (m_menuNum < 4) {
//    *size = char(m_menuNum);
//    for (int i = 0; i < 3; i++) {
//      arr[i] = i + 1 >= m_menuNum ? 0 : i;
//    }
//    return arr;
//  }
//  unsigned char i = 0;
//  *size = 4;
//  // 当前为第一个
//  if (m_menuIndex == 0) {
//    arr[i] = m_menuNum - 1;
//    i++;
//    arr[i] = 0;
//    i++;
//    arr[i] = 1;
//    i++;
//    arr[i] = 2;
//    i++;
//    return arr;
//  }
//  // 当前为最后一个
//  if (m_menuIndex == m_menuNum - 1) {
//    arr[i] = m_menuNum - 2;
//    i++;
//    arr[i] = m_menuNum - 1;
//    i++;
//    arr[i] = 0;
//    i++;
//    arr[i] = 1;
//    i++;
//    return arr;
//  }
//  // 当前为中间
//  arr[i] = m_menuIndex - 1;
//  i++;
//  arr[i] = m_menuIndex;
//  i++;
//  arr[i] = m_menuIndex + 1;
//  i++;
//  arr[i] = m_menuIndex + 2;
//  i++;
//  return arr;
//}

unsigned char *AppDesktop::getIndexArr(unsigned char *size) {
  auto arr = (unsigned char *) pvPortMalloc(5);
  memset(arr, 0, 5);
  // 不足3个
  if (m_menuNum < 3) {
    *size = char(m_menuNum);
    for (int i = 0; i < 3; i++) {
      arr[i] = i + 1 >= m_menuNum ? 0 : i;
    }
    return arr;
  }
  unsigned char i = 0;
  // 当前为第一个
  if (m_menuIndex == 0) {
    if (m_status == 1) {
      arr[i] = m_menuNum - 2;
      i++;
    }
    arr[i] = m_menuNum - 1;
    i++;
    arr[i] = 0;
    i++;
    arr[i] = 1;
    i++;
    if (m_status == 2 || m_status == 0) {
      arr[i] = 2;
      i++;
    }
    *size = i;
    return arr;
  }

  // 当前为最后一个
  if (m_menuIndex == m_menuNum - 1) {
    if (m_status == 1) {
      arr[i] = m_menuNum - 3;
      i++;
    }
    arr[i] = m_menuNum - 2;
    i++;
    arr[i] = m_menuIndex;
    i++;
    arr[i] = 0;
    i++;
    if (m_status == 2 || m_status == 0) {
      arr[i] = 1;
      i++;
    }
    *size = i;
    return arr;
  }

  // 当前为中间
  if (m_status == 1) {
    if (m_menuIndex == 1) {
      arr[i] = m_menuNum - 1;
    } else {
      arr[i] = m_menuIndex - 2;
    }
    i++;
  }
  arr[i] = m_menuIndex - 1;
  i++;
  arr[i] = m_menuIndex;
  i++;
  arr[i] = m_menuIndex + 1;
  i++;
  if (m_status == 2 || m_status == 0) {
    if (m_menuIndex + 1 == m_menuNum - 1) {
      arr[i] = 0;
    } else {
      arr[i] = m_menuIndex + 2;
    }
    i++;
  }
  *size = i;
  return arr;
}

void AppDesktop::SetShifting(float shifting) {
  m_shifting = shifting;
}

void AppDesktop::MenuDec() {
  // 上翻
  m_status = 2;
  Easing_Strat(EasingType <= 0 ? EasingType_CircularCurve_Out : EasingType, 35, 5, 0, 1, EasingCallBack, false, 1,
               (void *) this);
  m_status = 0;
  m_shifting = 0;
  if (m_menuIndex + 1 == m_menuNum) {
    m_menuIndex = 0;
    ReView();
    return;
  }
  m_menuIndex++;
  ReView();
}

void AppDesktop::MenuAdd() {
  // 往下滚动
  m_status = 1;
  Easing_Strat(EasingType <= 0 ? EasingType_CircularCurve_Out : EasingType, 35, 5, 0, 1, EasingCallBack, false, 0,
               (void *) this);
  m_status = 0;
  m_shifting = 0;
  if (m_menuIndex == 0) {
    m_menuIndex = m_menuNum - 1;
    ReView();
    return;
  }
  m_menuIndex--;
  ReView();
}

const desktopCallType *AppDesktop::GetDesktopInfo() {
  return &m_menuList[m_menuIndex];
}

unsigned char AppDesktop::GetMenuIndex() {
  return m_menuIndex;
}