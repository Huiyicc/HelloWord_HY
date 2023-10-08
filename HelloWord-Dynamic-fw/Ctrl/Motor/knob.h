#ifndef HELLOWORD_DYNAMIC_FW_KNOB_H
#define HELLOWORD_DYNAMIC_FW_KNOB_H

#include "motor.h"


class KnobSimulator {
public:
    KnobSimulator() = default;;

    enum Mode_t {
        // �ر�ģʽ
        MODE_DISABLE = 0,
        // ����ģʽ
        MODE_INERTIA,
        // ������ģʽ
        MODE_ENCODER,
        // ����ģʽ
        MODE_SPRING,
        // ����ģʽ
        MODE_DAMPED,
        // ����ģʽ
        MODE_PADDLE,
        // ��תģʽ
        MODE_SPIN
    };

    void Init(Motor *_motor);

    void Tick();

    void SetEnable(bool _en);

    void ApplyZeroPos(float _angle = 0);

    void SetMode(Mode_t _mode);

    void SetLimitPos(float _min, float _max);

    float GetPosition();

    float GetVelocity();

    int GetEncoderModePos();

    void SetEncoderModePos(int encoderDivides);

    Mode_t GetMode();

    void SetAnglePID(float p, float i, float d);

    void SetVelocityPID(float p, float i, float d);

    void SetTorqueLimit(float _val);

    float deviation = -3.7;
    float filterateMax = 0.04;
    float filterateVelocityMax = 0.7;
    float encoderDistance = 0;
    float encoderPosition = 0;
private:
    Motor *motor{};
    Mode_t mode = MODE_DISABLE;

    float zeroPosition = 0;
    float limitPositionMax = 5.1;
    float limitPositionMin = 3.3;
    int encoderDivides = 5;
    float lastAngle=0;
    float lastVelocity=0;
    float maxVelocity = 0;

    bool reset = false;
};

#endif //HELLOWORD_DYNAMIC_FW_KNOB_H
