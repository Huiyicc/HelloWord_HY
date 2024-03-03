#ifndef HELLOWORD_DYNAMIC_FW_KNOB_H
#define HELLOWORD_DYNAMIC_FW_KNOB_H

#include "motor.h"

struct AppKnobConfig;

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
        // ���ֱ�����ģʽ
        MODE_JINLUNENCODER,
        // ����ģʽ
        MODE_SPRING,
        // ����ģʽ
        MODE_DAMPED,
        // ����ģʽ
        MODE_PADDLE,
        // ��תģʽ
        MODE_SPIN,
        // ����ģʽ
        MODE_INTELLIGENT,
    };

    void Init(Motor *_motor);

    void Tick();

    void SetEnable(bool _en);

    void ApplyZeroPos(float _angle = 0);

    void SetMode(Mode_t _mode, const AppKnobConfig *cfg);

    void SetLimitPos(float _min, float _max);

    float GetPosition();

    float GetVelocity();

    int GetEncoderModePos();

    int GetEncoderModePos(float p);

    float GetPositionFromEncoderModePos(int pos);

    int GetEncoderDivides();

    float GetZeroPosition();

    void SetEncoderModePos(uint32_t encoderDivides);

    Mode_t GetMode();

    void SetAnglePID(float p, float i, float d);

    void SetVelocityPID(float p, float i, float d);

    void SetTorqueLimit(float _val);

    float GetTorqueLimit();

    void UpdateConf(const AppKnobConfig *cfg);

    float GetTarget();

    float deviation = -3.7;
    float filterateMax = 0.04;
    float filterateVelocityMax = 0.05;
    float triggerVelocityMax = 7;
    float encoderDistance = 0;
    float encoderPosition = 0;
    float lastEncoderPosition = 0;

    Motor *motor{};
    Mode_t mode = MODE_DISABLE;

    float zeroPosition = 0;
    float limitPositionMax = 5.1;
    float limitPositionMin = 3.3;
    int encoderDivides = 10;
    float lastAngle = 0;
    float lastVelocity = 0;
    float maxVelocity = 0;

    bool reset = false;
    bool pStatus = false;

    float torqueLimit = 1.0;
};

#endif //HELLOWORD_DYNAMIC_FW_KNOB_H
