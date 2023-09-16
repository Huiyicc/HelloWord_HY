#include <cmath>
#include <cstdio>
#include "knob.h"
#include "st_hardware.h"
#include "os_define.hpp"

void KnobSimulator::Init(Motor *_motor) {
    motor = _motor;
    motor->config.controlMode = Motor::ControlMode_t::TORQUE;
    motor->config.voltageLimit = 3.0;
    motor->config.velocityLimit = 100;
    motor->config.pidVelocity.p = 0.1;
    motor->config.pidVelocity.i = 0.0;
    motor->config.pidVelocity.d = 0.0;
    motor->config.pidAngle.p = 80;
    motor->config.pidAngle.i = 0;
    motor->config.pidAngle.d = 0.7;

    // When motor calibrated, we can replace Init with something like:
    // motor->Init(1.815850, EncoderBase::CW)
    if (motor->Init()) {
        printf("ZeroElectricAngleOffset: %f | Encoder direction: %s\n", motor->zeroElectricAngleOffset,
               motor->encoder->countDirection == EncoderBase::CW ? "CW" : "CCW");
        motor->target = 0;
        motor->SetEnable(false);
    } else
        printf("Error[%d]\n", motor->error);

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
            motor->SetTorqueLimit(1.2);
            motor->config.controlMode = Motor::VELOCITY;
            motor->config.pidVelocity.p = 6;
            motor->config.pidVelocity.i = 0.0;
            motor->config.pidVelocity.d = 0.0;
            motor->config.pidAngle.p = 0;
            motor->config.pidAngle.i = 0;
            motor->config.pidAngle.d = 0;
            motor->target = 0;
        }
            break;
        case MODE_ENCODER: {
            motor->SetEnable(true);
            motor->SetTorqueLimit(0.8);
            motor->config.controlMode = Motor::ControlMode_t::ANGLE;
            motor->config.pidVelocity.p = 0.02;
            motor->config.pidVelocity.i = 0.0;
            motor->config.pidVelocity.d = 0.0;
            motor->config.pidAngle.p = 100;
            motor->config.pidAngle.i = 0.3;
            motor->config.pidAngle.d = 3;
            motor->target = zeroPosition;
            lastAngle = zeroPosition;
            encoderDistance = _2PI / float(encoderDivides);
//            auto m = fmodf(motor->target, motor->target);
//            if (fabs(m) < filterateMax) {
//                motor->target += m;
//            }
//            lastAngle = motor->target;
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
            motor->config.pidAngle.i = 0.3;
            motor->config.pidAngle.d = 3;
            motor->target = zeroPosition;
        }
            break;
    }
}


void KnobSimulator::Tick() {
    switch (mode) {
        case MODE_INERTIA: {
            auto v = GetVelocity();
            auto a = GetPosition();
            if(fabsf(v-lastVelocity)>0.5)motor->target = v;
            else if(fabsf(v-lastVelocity)<0.08)motor->target = v*0.001F+lastVelocity*0.999F;
            else motor->target = v*fabsf(v-lastVelocity)*0.5F+lastVelocity*(1-fabsf(v-lastVelocity)*0.5F);
            lastVelocity = motor->target;

            if(fabsf(lastAngle-a)>0.2) {
                if (v > 16)v = 16;
                else if (v < -16)v = -16;
                if (v > -0.05 && v < 0.05)v = 0;
                else if (v < 1 && v > 0)v = 1;
                else if (v < 0 && v > -1)v = -1;
                lastAngle = a;
            }
        }
            break;
        case MODE_ENCODER: {
            // 当前位置
            auto a = GetPosition();
            if (std::fabs(a) < filterateMax&&!reset) {
                motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
                motor->target = 0;
                lastAngle = a;
                encoderPosition = GetEncoderModePos();
                break;
            } else {
                auto fm = std::fabs(fmod(a,encoderDistance));
                if (fm<filterateMax||reset) {
                    if (reset&&fm<0.01) {
                        reset = false;
                        motor->config.controlMode = Motor::ControlMode_t::VELOCITY;
                        motor->target = 0;
                        lastAngle = a;
                        encoderPosition = GetEncoderModePos();
                        break;
                    }
                }
                reset = true;
                motor->config.controlMode = Motor::ControlMode_t::ANGLE;
                motor->target = lastAngle+zeroPosition;
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
        case MODE_DISABLE:
        case MODE_SPRING:
        case MODE_SPIN:
            break;
        case MODE_PADDLE:{
            auto a = GetPosition();
            auto fa = std::fabs(a);
            if (fa < filterateMax || reset) {
                if (fa>filterateMax) {
                    reset= true;
                    motor->config.controlMode = Motor::ControlMode_t::ANGLE;
                    motor->target = zeroPosition;
                    break;
                }
                if (reset&&fa<0.01) {
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


int KnobSimulator::GetEncoderModePos() {
    return std::lround(GetPosition() / (_2PI / (float) encoderDivides));
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

void KnobSimulator::SetVelocityPID(float p,float i,float d) {
    motor->config.pidVelocity.p = p;
    motor->config.pidVelocity.i = i;
    motor->config.pidVelocity.d = d;
}

void KnobSimulator::SetAnglePID(float p,float i,float d) {
    motor->config.pidAngle.p =p;
    motor->config.pidAngle.i = i;
    motor->config.pidAngle.d = d;
}

void KnobSimulator::SetTorqueLimit(float _val) {
    motor->SetTorqueLimit(_val);
}