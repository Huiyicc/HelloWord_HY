#ifndef HELLOWORD_DYNAMIC_FW_KNOB_H
#define HELLOWORD_DYNAMIC_FW_KNOB_H

#include "motor.h"


class KnobSimulator {
public:
    KnobSimulator() = default;;

    enum Mode_t {
        MODE_DISABLE = 0,
        MODE_INERTIA,
        MODE_ENCODER,
        MODE_SPRING,
        MODE_DAMPED,
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

    float deviation = -3.7;
    float filterateMax = 0.04;
    float encoderDistance = 0;
    float encoderPosition = 0;
private:
    Motor *motor{};
    Mode_t mode = MODE_DISABLE;

    float zeroPosition = -3.7;
    float limitPositionMax = 5.1;
    float limitPositionMin = 3.3;
    int encoderDivides = 5;

    float lastAngle{};
    float lastVelocity{};

    bool reset = false;
};

#endif //HELLOWORD_DYNAMIC_FW_KNOB_H
