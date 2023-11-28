#include <cmath>
#include <cstdio>
#include "knob.h"
#include "st_hardware.h"
#include "os_define.hpp"
#include "os/storage.hpp"

void KnobSimulator::Init(Motor *_motor) {
  zeroPosition = GetSysConfig()->devices.knob.zeroPosition;
  motor = _motor;
  motor->config.controlMode = Motor::ControlMode_t::TORQUE;
  motor->config.voltageLimit = 1.5;
  motor->config.velocityLimit = 100.0f;
  motor->config.pidVelocity.p = 0.1;
  motor->config.pidVelocity.i = 0.0;
  motor->config.pidVelocity.d = 0.0;
  motor->config.pidAngle.p = 80;
  motor->config.pidAngle.i = 0;
  motor->config.pidAngle.d = 0.7;

  // When motor calibrated, we can replace Init with something like:
  // motor->Init(1.815850, EncoderBase::CW)
  for (char i = 0; i < 4; ++i) {
    g_sysCtx->Device.ctrl.Action = motor->Init();
    if (g_sysCtx->Device.ctrl.Action) {
      break;
    } else {
      OLED_CLEAR_BUFFER();
      OLED_DEVICES()->SetDrawColor(1);
      OLED_DEVICES()->DrawBox(6, 11, 20, 20);
      OLED_DEVICES()->DrawUTF8(9, 41, "重");
      OLED_DEVICES()->DrawUTF8(9, 55, "新");
      OLED_DEVICES()->DrawUTF8(9, 69, "校");
      OLED_DEVICES()->DrawUTF8(9, 83, "准");
      OLED_DEVICES()->DrawUTF8(9, 97, "电");
      OLED_DEVICES()->DrawUTF8(9, 111, "机");
      OLED_DEVICES()->SetDrawColor(0);
      OLED_DEVICES()->DrawStr(6 + 3, 15, "HY");
      OLED_SEND_BUFFER();
    }
    if (i >= 3) {
      return;
    }
  }

  if (g_sysCtx->Device.ctrl.Action) {
//        printf("ZeroElectricAngleOffset: %f | Encoder direction: %s\n", motor->zeroElectricAngleOffset,
//               motor->encoder->countDirection == EncoderBase::CW ? "CW" : "CCW");
    motor->target = 0;
    motor->SetEnable(false);
  } else {
    //printf("Error[%d]\n", motor->error);
  }
}


void KnobSimulator::SetMode(KnobSimulator::Mode_t _mode) {
  mode = _mode;

  lastAngle = GetPosition();
  lastVelocity = GetVelocity();

  switch (mode) {
    case MODE_DISABLE:
      motor->SetEnable(false);
      break;
    case MODE_INERTIA: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(1.5);
      motor->config.controlMode = Motor::VELOCITY;
      motor->config.pidVelocity.p = 0.5;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->config.pidAngle.p = 20;
      motor->config.pidAngle.i = 0;
      motor->config.pidAngle.d = 0.7;
      motor->target = 0;
    }
      break;
    case MODE_ENCODER: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(0.2f);
      //zeroPosition=0;
      motor->config.controlMode = Motor::ControlMode_t::ANGLE;
      motor->config.pidVelocity.p = 0.02;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->config.pidAngle.p = 60;
      motor->config.pidAngle.i = 0;
      motor->config.pidAngle.d = 3;
      motor->target = zeroPosition;
      lastAngle = zeroPosition;
      encoderDistance = _2PI / float(encoderDivides);
    }
      break;
    case MODE_JINLUNENCODER: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(0.3f);
      //zeroPosition=0;
      motor->config.controlMode = Motor::ControlMode_t::ANGLE;
      motor->config.pidVelocity.p = 0.02;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->config.pidAngle.p = 80;
      motor->config.pidAngle.i = 0;
      motor->config.pidAngle.d = 3.5f;
      motor->target = zeroPosition;
      lastAngle = zeroPosition;
    }
      break;
    case MODE_SPRING: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(1.5);
      motor->config.controlMode = Motor::ControlMode_t::ANGLE;
      motor->config.pidVelocity.p = 0.1;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->config.pidAngle.p = 100;
      motor->config.pidAngle.i = 0;
      motor->config.pidAngle.d = 3.5;
      motor->target = 4.2;
    }
      break;
    case MODE_DAMPED: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(1.5);
      motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
      motor->config.pidVelocity.p = 0.1;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->target = zeroPosition;
    }
      break;
    case MODE_SPIN: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(1.5);
      motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
      motor->config.pidVelocity.p = 0.3;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->target = 20;
    }
      break;
    case MODE_PADDLE: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(1.0);
      motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
      motor->config.pidVelocity.p = 0.02;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->config.pidAngle.p = 100;
      motor->config.pidAngle.i = 0;
      motor->config.pidAngle.d = 3.5f;
      motor->target = zeroPosition;
    }
      break;
    case MODE_INTELLIGENT: {
      motor->SetEnable(true);
      motor->SetTorqueLimit(0.4f);
      motor->config.controlMode = Motor::ControlMode_t::ANGLE;
      motor->config.pidVelocity.p = 0.02;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      motor->config.pidAngle.p = 100;
      motor->config.pidAngle.i = 0;
      motor->config.pidAngle.d = 3.5;
      motor->target = zeroPosition;
      lastAngle = zeroPosition;
      encoderDistance = _2PI / float(encoderDivides);
    }
      break;
  }
  motor->encoder->vel = (motor->config.controlMode == Motor::ControlMode_t::VELOCITY);
}


void KnobSimulator::Tick() {
  switch (mode) {
    case MODE_INERTIA: {
      motor->config.pidVelocity.p = 0.5;
      motor->config.pidVelocity.i = 0.0;
      motor->config.pidVelocity.d = 0.0;
      float v = GetVelocity();
      float a = v - lastVelocity;
      if (v == 0.0f || std::fabs(v) < filterateVelocityMax) {
        motor->config.pidVelocity.p = 0.02;
        motor->config.pidVelocity.i = 0.0;
        motor->config.pidVelocity.d = 0.0;
        motor->target = 0.0f;
        maxVelocity = 0.0f;
      } else if (v > 0.0f) {
        if (a > 1.0f || v > maxVelocity) {
          motor->target = v;
          maxVelocity = v;
        } else if (a < -2.0f) {
          motor->target += a;
          if (motor->target < 1.0f) {
            motor->target = 0.0f;
            maxVelocity = 0.0f;
          }
        } else {
          motor->target -= 0.001f;
        }
      } else if (v < 0.0f) {
        if (a < -1.0f || v < maxVelocity) {
          motor->target = v;
          maxVelocity = v;
        } else if (a > 2.0f) {
          motor->target += a;
          if (motor->target > -1.0f) {
            motor->config.pidVelocity.p = 0.02;
            motor->config.pidVelocity.i = 0.0;
            motor->config.pidVelocity.d = 0.0;
            motor->target = 0.0f;
            maxVelocity = 0.0f;
          }
        } else {
          motor->target += 0.001f;
        }
      }
      lastVelocity = motor->target;
    }
      break;
    case MODE_ENCODER: {
      auto a = GetPosition();
      // 如果当前位置小于阈值filterateMax的绝对值，并且不需要重置(reset为false)
      if (std::fabs(a) < filterateMax && !reset) {
        // 设置电机控制模式为速度模式
        motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
        // 将电机的目标速度设置为0
        motor->target = 0;
        // 更新上一次角度为当前位置a
        lastAngle = a;
        // 获取编码器模式的位置
        lastEncoderPosition = encoderPosition;
        encoderPosition = GetEncoderModePos();
        break;
      } else {
        // 计算当前位置a对encoderDistance取模
        // 绝对值
        auto fmabs = std::fabs(fmod(a, encoderDistance));

        // 如果fm小于阈值filterateMax或者需要重置(reset为true)
        if (fmabs < filterateMax || reset) {
          // 如果需要重置并且fm小于0.01
          if (reset && fmabs < 0.01) {
            // 取消重置标志
            reset = false;
            // 设置电机控制模式为速度模式
            motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
            // 将电机的目标速度设置为0
            motor->target = 0;
            // 更新上一次角度为当前位置a
            lastAngle = a;
            // 获取编码器模式的位置
            lastEncoderPosition = encoderPosition;
            encoderPosition = GetEncoderModePos();
            break;
          }
        }
        // 设置重置标志为true
        reset = true;

        motor->config.controlMode = Motor::ControlMode_t::ANGLE;
        auto angDiff = a - lastAngle;
        if (angDiff > 0) {
          if (angDiff > _PI / (float) encoderDivides) {
            motor->target += _2PI / (float) encoderDivides + zeroPosition;
            lastAngle += _2PI / (float) encoderDivides;
          } else {
            motor->target = lastAngle + zeroPosition;
          }
        } else if (angDiff < 0) {
          if (angDiff < -_PI / (float) encoderDivides) {
            motor->target -= _2PI / (float) encoderDivides + zeroPosition;
            lastAngle -= _2PI / (float) encoderDivides;
          } else {
            motor->target = lastAngle + zeroPosition;
          }
        } else {
          motor->target = lastAngle + zeroPosition;
        }

        break;
      }
    }
      break;
    case MODE_JINLUNENCODER: {
      auto a = GetPosition();
      // 如果当前位置小于阈值filterateMax的绝对值，并且不需要重置(reset为false)
      if (std::fabs(a) < filterateMax && !reset) {
        // 设置电机控制模式为速度模式
        motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
        // 将电机的目标速度设置为0
        motor->target = 0;
        // 更新上一次角度为当前位置a
        lastAngle = a;
        // 获取编码器模式的位置
        encoderPosition = GetEncoderModePos();
        break;
      } else {
        // 计算当前位置a对encoderDistance取模
        // 绝对值
        auto fmabs = std::fabs(fmod(a, encoderDistance));

        // 如果fm小于阈值filterateMax或者需要重置(reset为true)
        if (fmabs < filterateMax || reset) {
          // 如果需要重置并且fm小于0.01
          if (reset && fmabs < 0.01) {
            // 取消重置标志
            reset = false;
            // 设置电机控制模式为速度模式
            motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
            // 将电机的目标速度设置为0
            motor->target = 0;
            // 更新上一次角度为当前位置a
            lastAngle = a;
            // 获取编码器模式的位置
            encoderPosition = GetEncoderModePos();
            break;
          }
        }

        // 设置重置标志为true
        reset = true;
        // 设置电机控制模式为角度模式
        motor->config.controlMode = Motor::ControlMode_t::ANGLE;
        motor->target = lastAngle + zeroPosition;

        break;
      }
    }
      break;
    case MODE_DAMPED:
      if (limitPositionMax != 0 && limitPositionMin != 0) {
        auto a = GetPosition();
        if (a > limitPositionMax) {
          motor->config.controlMode = Motor::ANGLE;
          motor->target = limitPositionMax;
        } else if (a < limitPositionMin) {
          motor->config.controlMode = Motor::ANGLE;
          motor->target = limitPositionMin;
        } else {
          motor->config.controlMode = Motor::VELOCITY;
          motor->target = 0;
        }
      }
      break;
    case MODE_INTELLIGENT: {
      encoderPosition = GetEncoderModePos();
      float limitVelocity = 0.1f;
      auto v = GetVelocity();
      auto fv = fabs(v);
      if ((fv > triggerVelocityMax || pStatus) && fv > limitVelocity) {
        reset = false;
        motor->config.controlMode = Motor::VELOCITY;
        if (!pStatus) {
          pStatus = true;
          motor->SetTorqueLimit(1.5);
          motor->config.pidVelocity.p = 0.5;
          motor->config.pidVelocity.i = 0.0;
          motor->config.pidVelocity.d = 0.0;
          motor->config.pidAngle.p = 20;
          motor->config.pidAngle.i = 0;
          motor->config.pidAngle.d = 0.7;
        }
        // 速度控制模式
        float a = v - lastVelocity;
        if (v == 0.0f) {
          motor->target = 0.0f;
          maxVelocity = 0.0f;
        } else if (v > 0.0f) {
          if (a > 1.0f || v > maxVelocity) {
            motor->target = v;
            maxVelocity = v;
          } else if (a < -2.0f) {
            motor->target += a;
            if (motor->target < 1.0f) {
              motor->target = 0.0f;
              maxVelocity = 0.0f;
            }
          } else {
            motor->target -= 0.001f;
          }
        } else if (v < 0.0f) {
          if (a < -1.0f || v < maxVelocity) {
            motor->target = v;
            maxVelocity = v;
          } else if (a > 2.0f) {
            motor->target += a;
            if (motor->target > -1.0f) {
              motor->target = 0.0f;
              maxVelocity = 0.0f;
            }
          } else {
            motor->target += 0.001f;
          }
        }
        lastVelocity = motor->target;
        break;
      }
      if (fv < limitVelocity || !pStatus) {
        if (pStatus) {
          motor->SetTorqueLimit(0.4f);
          motor->config.controlMode = Motor::ControlMode_t::ANGLE;
          motor->config.pidVelocity.p = 0.02;
          motor->config.pidVelocity.i = 0.0;
          motor->config.pidVelocity.d = 0.0;
          motor->config.pidAngle.p = 100;
          motor->config.pidAngle.i = 0;
          motor->config.pidAngle.d = 3.5;
          motor->target = zeroPosition;
          lastAngle = zeroPosition;
          encoderDistance = _2PI / float(encoderDivides);
        }
        pStatus = false;
        auto a = GetPosition();
        // 如果当前位置小于阈值filterateMax的绝对值，并且不需要重置(reset为false)
        if (std::fabs(a) < filterateMax && !reset) {
          // 设置电机控制模式为速度模式
          motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
          // 将电机的目标速度设置为0
          motor->target = 0;
          // 更新上一次角度为当前位置a
          lastAngle = a;
        } else {
          // 计算当前位置a对encoderDistance取模
          // 绝对值
          auto fmabs = std::fabs(fmod(a, encoderDistance));

          // 如果fm小于阈值filterateMax或者需要重置(reset为true)
          if (fmabs < filterateMax || reset) {
            // 如果需要重置并且fm小于0.01
            if (reset && fmabs < 0.01) {
              // 取消重置标志
              reset = false;
              // 设置电机控制模式为速度模式
              motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
              // 将电机的目标速度设置为0
              motor->target = 0;
              // 更新上一次角度为当前位置a
              lastAngle = a;
              break;
            }
          }

          // 设置重置标志为true
          reset = true;

          motor->config.controlMode = Motor::ControlMode_t::ANGLE;
          auto angDiff = a - lastAngle;
          if (angDiff > 0) {
            if (angDiff > _PI / (float) encoderDivides) {
              motor->target += _2PI / (float) encoderDivides + zeroPosition;
              lastAngle += _2PI / (float) encoderDivides;
            } else {
              motor->target = lastAngle + zeroPosition;
            }
          } else if (angDiff < 0) {
            if (angDiff < -_PI / (float) encoderDivides) {
              motor->target -= _2PI / (float) encoderDivides + zeroPosition;
              lastAngle -= _2PI / (float) encoderDivides;
            } else {
              motor->target = lastAngle + zeroPosition;
            }
          } else {
            motor->target = lastAngle + zeroPosition;
          }

          break;
        }
      }
    }
      break;
    case MODE_DISABLE:
    case MODE_SPRING:
    case MODE_SPIN:
      break;
    case MODE_PADDLE: {
      auto a = GetPosition();
      auto fa = std::fabs(a);
      if (fa < filterateMax || reset) {
        if (fa > filterateMax) {
          reset = true;
          motor->config.controlMode = Motor::ControlMode_t::ANGLE;
          motor->target = zeroPosition;
          break;
        }
        if (reset && fa < 0.01) {
          reset = false;
          motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
          motor->target = 0;
          lastAngle = a;
        }
        break;
      }
      motor->config.controlMode = Motor::ControlMode_t::ANGLE;
      motor->target = zeroPosition;
    }
      break;
  }

  motor->Tick();
}


void KnobSimulator::SetLimitPos(float _min, float _max) {
  limitPositionMin = _min;
  limitPositionMax = _max;
}


void KnobSimulator::ApplyZeroPos(float _angle) {
  if (_angle != 0)
    zeroPosition = _angle;
  else
    zeroPosition = motor->GetEstimateAngle();
}


float KnobSimulator::GetPosition() {
  return motor->GetEstimateAngle() - zeroPosition;
}

float KnobSimulator::GetVelocity() {
  return motor->GetEstimateVelocity();
}

float KnobSimulator::GetZeroPosition() {
  return zeroPosition;
}

int KnobSimulator::GetEncoderModePos() {
  return std::lround(GetPosition() / (_2PI / (float) encoderDivides));
}

int KnobSimulator::GetEncoderDivides() {
  return encoderDivides;
}

void KnobSimulator::SetEncoderModePos(int EncoderDivides) {
  // (Angle / _2PI) * 360° = AngleRaw
  // 逆向公式
  // Angle = (AngleRaw / 360°) * _2PI
  encoderDivides = EncoderDivides;
  encoderDistance = _2PI / float(EncoderDivides);
}

void KnobSimulator::SetEnable(bool _en) {
  SetPowerMotor(_en);
  motor->SetEnable(_en);
}

KnobSimulator::Mode_t KnobSimulator::GetMode() { return mode; }

void KnobSimulator::SetVelocityPID(float p, float i, float d) {
  motor->config.pidVelocity.p = p;
  motor->config.pidVelocity.i = i;
  motor->config.pidVelocity.d = d;
}

void KnobSimulator::SetAnglePID(float p, float i, float d) {
  motor->config.pidAngle.p = p;
  motor->config.pidAngle.i = i;
  motor->config.pidAngle.d = d;
}

void KnobSimulator::SetTorqueLimit(float _val) {
  torqueLimit = _val;
  motor->SetTorqueLimit(_val);
}

float KnobSimulator::GetTorqueLimit() {
  return torqueLimit;
}
