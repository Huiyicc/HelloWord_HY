//
// Created by Huiyicc on 2023/9/1.
// 针对瀚文项目自https://github.com/Huiyicc/Easingcpp.git移植
//
#include "Easingcpp.hpp"
#include "cmsis_os.h"

double Easing_asin(double value_);

double Easing_getInBounce(double begin_, double end_, double percent_);

double Easing_getOutBounce(double begin_, double end_, double percent_);

//==========easobj===================
//线性运动
double _EasingType_Linear(double &percents_, double _beginvalue, double _endvalue) {
  //percents_ 比例，0~1
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return (_endvalue - _beginvalue) * percents_ + _beginvalue;
}

//圆线性插值
double _EasingType_CircularLinearInterpolation(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed = end - start;
  double _min = 0;
  double _max = 360;
  double _half = 180;
  double _retval = 0;
  double _diff = 0;
  if (_ed < -_half) {
    _diff = (_max - start + end) * percents_;
    _retval = start + _diff;
  } else {
    if (_ed > _half) {
      _diff = -(_max - end + start) * percents_;
      _retval = start + _diff;
    } else {
      _retval = start + _ed * percents_;
    }
  }
  return _retval;
}

//弹性缓动
double _EasingType_Elasticity(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st =
      (sin(percents_ * PI * (0.2 + 2.5 * percents_ * percents_ * percents_)) * pow(1 - percents_, 2.2) + percents_) *
      (1 + 1.2 * (1 - percents_));
  return start + (end - start) * _st;
}

//冲击缓动
double _EasingType_Impact(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  if (percents_ == 0) { return 0; }
  if (percents_ == 1) { return 0; }
  double _p = 0.3;
  double _s = _p / (2 * PI) * Easing_asin(0);
  return end * pow(2, -10 * percents_) * sin((percents_ - _s) * 2 * PI / _p);
}

//二次方缓动
double _EasingType_Quadratic_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return (end - start) * percents_ * percents_ + start;
}

//二次方缓动_In
double _EasingType_Quadratic_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return -(end - start) * percents_ * (percents_ - 2) + start;
}

//二次方缓动_InOut
double _EasingType_Quadratic_InOut(double percents_, double start, double end) {
  double _ts;
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  _ts = percents_ * 2;
  if (_ts < 1) { return (end - start) / 2 * _ts * _ts + start; }
  _ts = _ts - 1;
  return -(end - start) / 2 * (_ts * (_ts - 2) - 1) + start;
}

//三次方缓动_In
double _EasingType_Cube_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return (end - start) * percents_ * percents_ * percents_ + start;
}

//三次方缓动_Out
double _EasingType_Cube_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st;
  _st = percents_ - 1;
  return (end - start) * (_st * _st * _st + 1) + start;
}

//三次方缓动_InOut
double _EasingType_Cube_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st;
  double _ed1;
  _st = percents_ * 2;
  _ed1 = end - start;
  if (_st < 1) { return _ed1 / 2 * _st * _st * _st + start; }
  _st = _st - 2;
  return _ed1 / 2 * (_st * _st * _st + 2) + start;
}

//四次方缓动_In
double _EasingType_Biquadrate_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return (end - start) * percents_ * percents_ * percents_ * percents_ + start;
}

//四次方缓动_Out
double _EasingType_Biquadrate_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st;
  _st = percents_ - 1;
  return -(end - start) * (_st * _st * _st * _st - 1) + start;
}

//四次方缓动_InOut
double _EasingType_Biquadrate_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st;
  double _ed1;
  _st = percents_ * 2;
  _ed1 = end - start;
  if (_st < 1) { return _ed1 / 2 * _st * _st * _st * _st + start; }
  _st = _st - 2;
  return -_ed1 / 2 * (_st * _st * _st * _st - 2) + start;
}

//五次方缓动_In
double _EasingType_Fifth_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return (end - start) * percents_ * percents_ * percents_ * percents_ * percents_ + start;
}

//五次方缓动_Out
double _EasingType_Fifth_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st = percents_ - 1;
  double _ed1 = end - start;
  return _ed1 * (_st * _st * _st * _st * _st + 1) + start;
}

//五次方缓动_InOut
double _EasingType_Fifth_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st = percents_ * 2;
  double _ed1 = end - start;
  if (_st < 1) { return _ed1 / 2 * _st * _st * _st * _st * _st + start; }
  _st = _st - 2;
  return _ed1 / 2 * (_st * _st * _st * _st * _st + 2) + start;
}

//正弦曲线缓动
double _EasingType_Sinusoid_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed1 = end - start;
  return -_ed1 * cos(percents_ / 1 * PI / 2) + _ed1 + start;
}

//正弦曲线缓动_InOut
double _EasingType_Sinusoid_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed1 = end - start;
  return _ed1 * sin(percents_ / 1 * PI / 2) + start;
}

//正弦曲线缓动_InOut
double _EasingType_Sinusoid_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed1 = end - start;
  return -_ed1 / 2 * (cos(PI * percents_ / 1) - 1) + start;
}

//指数曲线缓动_In
double _EasingType_IndexCurve_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed1 = end - start;
  return _ed1 * pow(2, 10 * (percents_ / 1 - 1)) + start;
}

//指数曲线缓动_Out
double _EasingType_IndexCurve_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed1 = end - start;
  return _ed1 * (-pow(2, -10 * percents_ / 1) + 1) + start;
}

//指数曲线缓动_InOut
double _EasingType_IndexCurve_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st = percents_ * 2;
  double _ed1 = end - start;
  if (_st < 1) { return _ed1 / 2 * pow(2, 10 * (_st - 1)) + start; }
  _st = _st - 1;
  return _ed1 / 2 * (-pow(2, -10 * _st) + 2) + start;
}

//圆曲线缓动_In
double _EasingType_CircularCurve_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed1 = end - start;
  return -_ed1 * (sqrt(1 - percents_ * percents_) - 1) + start;
}

//圆曲线缓动_Out
double _EasingType_CircularCurve_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st = percents_ - 1;
  double _ed1 = end - start;
  return _ed1 * sqrt(1 - _st * _st) + start;
}

//圆曲线缓动_InOut
double _EasingType_CircularCurve_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _st = percents_ * 2;
  double _ed1 = end - start;
  if (_st < 1) { return -_ed1 / 2 * (sqrt(1 - _st * _st) - 1) + start; }
  _st = _st - 2;
  return _ed1 / 2 * (sqrt(1 - _st * _st) + 1) + start;
}

//反弹缓动_指数衰减_In
double _EasingType_Rebound_Damping_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return Easing_getInBounce(start, end, percents_);
}

//反弹缓动_指数衰减_Out
double _EasingType_Rebound_Damping_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  return Easing_getOutBounce(start, end, percents_);
}

//反弹缓动_指数衰减_InOut
double _EasingType_Rebound_Damping_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed = end - start;
  if (percents_ < 0.5) { return Easing_getInBounce(0, _ed, percents_ * 2) * 0.5 + start; }
  return Easing_getOutBounce(0, _ed, percents_ * 2 - 1) * 0.5 + _ed * 0.5 + start;
}

//三次方缓动_溢出_In
double _EasingType_Cube_Spill_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed = end - start;
  double _s = 1.70158;
  return _ed * percents_ * percents_ * ((_s + 1) * percents_ - _s) + start;
}

//三次方缓动_溢出_Out
double _EasingType_Cube_Spill_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed = end - start;
  double _s = 1.70158;
  double _st = percents_ - 1;
  return _ed * (_st * _st * ((_s + 1) * _st + _s) + 1) + start;
}

//三次方缓动_溢出_InOut
double _EasingType_Cube_Spill_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  double _ed = end - start;
  double _s = 1.70158;
  double _st = percents_ * 2;
  if (_st < 1) {
    _s = _s * 1.525;
    return _ed / 2 * _st * _st * ((_s + 1) * _st - _s) + start;
  }
  _st = _st - 2;
  _s = _s * 1.525;
  return _ed / 2 * (_st * _st * ((_s + 1) * _st + _s) + 2) + start;
}

//正弦曲线缓动_指数衰减_In
double _EasingType_Sinusoid_Damping_In(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  if (percents_ == 0) { return start; }
  if (percents_ == 1) { return end; }
  double _ed = end - start;
  double _d = 1;
  double _p = _d * 0.3;
  double _s = 0;
  double _a = 0;
  if (_a == 0 || _a < abs(_ed)) {
    _a = _ed;
    _s = _p / 4;
  } else {
    _s = _p / (2 * PI) * Easing_asin(_ed / _a);
  }
  double _st = percents_ - 1;
  return -(_a * pow(2, 10 * _st) * sin((_st * _d - _s) * 2 * PI / _p)) + start;
}

//正弦曲线缓动_指数衰减_Out
double _EasingType_Sinusoid_Damping_Out(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  if (percents_ == 0) { return start; }
  if (percents_ == 1) { return end; }
  double _ed = end - start;
  double _d = 1;
  double _p = _d * 0.3;
  double _s = 0;
  double _a = 0;
  if (_a == 0 || _a < abs(_ed)) {
    _a = _ed;
    _s = _p / 4;
  } else {
    _s = _p / (2 * PI) * Easing_asin(_ed / _a);
  }
  return _a * pow(2, -10 * percents_) * sin((percents_ * _d - _s) * 2 * PI / _p) + end;
}

//正弦曲线缓动_指数衰减_InOut
double _EasingType_Sinusoid_Damping_InOut(double percents_, double start, double end) {
  if (percents_ < 0) { percents_ = 0; }
  if (percents_ > 1) { percents_ = 1; }
  if (percents_ == 0) { return start; }
  double _st = percents_ * 2;
  if (_st == 2) { return end; }
  double _ed = end - start;
  double _d = 1;
  double _p = _d * 0.3;
  double _s = 0;
  double _a = 0;
  if (_a == 0 || _a < abs(_ed)) {
    _a = _ed;
    _s = _p / 4;
  } else {
    _s = _p / (2 * PI) * Easing_asin(_ed / _a);
  }
  if (_st < 1) {
    _st = _st - 1;
    return -0.5 * _a * pow(2, 10 * _st) * sin((_st * _d - _s) * 2 * PI / _p) + start;
  }
  _st = _st - 1;
  return _a * pow(2, -10 * _st) * sin((_st * _d - _s) * 2 * PI / _p) * 0.5 + end;
}


//======eashaper========

double Easing_getInBounce(double begin_, double end_, double percent_) {
  double _ed = end_ - begin_;
  return _ed - Easing_getOutBounce(0, _ed, 1 - percent_) + begin_;
}

double Easing_getOutBounce(double begin_, double end_, double percent_) {
  double _st = end_ - begin_;
  double _ed = percent_;
  if (_st < 1 / 2.75) { return _ed * 7.5625 * _st * _st + begin_; }
  if (_st < 2 / 2.75) {
    _st = _st - 1.5 / 2.75;
    return _ed * (7.5625 * _st * _st + 0.75) + begin_;
  }
  if (_st < 2.5 / 2.75) {
    _st = _st - 2.25 / 2.75;
    return _ed * (7.5625 * _st * _st + 0.9375) + begin_;
  }
  _st = _st - 2.625 / 2.75;
  return _ed * (7.5625 * _st * _st + 0.984375) + begin_;
}

double Easing_asin(double value_) {
  if (value_ < -1 || value_ > 1) { return 0; }
  return atan(value_ / sqrt(1 - value_ * value_));
}

bool Easing_GetProgress(int uType, double start, double end, double pro, double &cur) {
  switch (uType) {
    case EasingType_Linear://线性运动
      cur = _EasingType_Linear(pro, start, end);
      break;
    case EasingType_CircularLinearInterpolation://圆线性插值运动
      cur = _EasingType_Linear(pro, start, end);
      break;
    case EasingType_Elasticity://弹性缓动
      cur = _EasingType_Elasticity(pro, start, end);
      break;
    case EasingType_Impact://冲击缓动
      cur = _EasingType_Impact(pro, start, end);
      break;
    case EasingType_Quadratic_In://二次方缓动
      cur = _EasingType_Quadratic_In(pro, start, end);
      break;
    case EasingType_Quadratic_Out://二次方缓动_In
      cur = _EasingType_Quadratic_Out(pro, start, end);
      break;
    case EasingType_Quadratic_InOut://二次方缓动_InOut
      cur = _EasingType_Quadratic_InOut(pro, start, end);
      break;
    case EasingType_Cube_In://三次方缓动_In
      cur = _EasingType_Cube_In(pro, start, end);
      break;
    case EasingType_Cube_Out://三次方缓动_Out
      cur = _EasingType_Cube_Out(pro, start, end);
      break;
    case EasingType_Cube_InOut://三次方缓动_InOut
      cur = _EasingType_Cube_InOut(pro, start, end);
      break;
    case EasingType_Biquadrate_In://四次方缓动_In
      cur = _EasingType_Biquadrate_In(pro, start, end);
      break;
    case EasingType_Biquadrate_Out://四次方缓动_Out
      cur = _EasingType_Biquadrate_Out(pro, start, end);
      break;
    case EasingType_Biquadrate_InOut://四次方缓动_InOut
      cur = _EasingType_Biquadrate_InOut(pro, start, end);
      break;
    case EasingType_Fifth_In://五次方缓动_In
      cur = _EasingType_Fifth_In(pro, start, end);
      break;
    case EasingType_Fifth_Out://五次方缓动_Out
      cur = _EasingType_Fifth_Out(pro, start, end);
      break;
    case EasingType_Fifth_InOut://五次方缓动_InOut
      cur = _EasingType_Fifth_InOut(pro, start, end);
      break;
    case EasingType_Sinusoid_In://正弦曲线缓动
      cur = _EasingType_Sinusoid_In(pro, start, end);
      break;
    case EasingType_Sinusoid_Out://正弦曲线缓动_InOut
      cur = _EasingType_Sinusoid_Out(pro, start, end);
      break;
    case EasingType_Sinusoid_InOut://正弦曲线缓动_InOut
      cur = _EasingType_Sinusoid_InOut(pro, start, end);
      break;
    case EasingType_IndexCurve_In://指数曲线缓动_In
      cur = _EasingType_IndexCurve_In(pro, start, end);
      break;
    case EasingType_IndexCurve_Out://指数曲线缓动_Out
      cur = _EasingType_IndexCurve_Out(pro, start, end);
      break;
    case EasingType_IndexCurve_InOut://指数曲线缓动_InOut
      cur = _EasingType_IndexCurve_InOut(pro, start, end);
      break;
    case EasingType_CircularCurve_In://圆曲线缓动_In
      cur = _EasingType_CircularCurve_In(pro, start, end);
      break;
    case EasingType_CircularCurve_Out://圆曲线缓动_Out
      cur = _EasingType_CircularCurve_Out(pro, start, end);
      break;
    case EasingType_CircularCurve_InOut://圆曲线缓动_InOut
      cur = _EasingType_CircularCurve_InOut(pro, start, end);
      break;
    case EasingType_Rebound_Damping_In://反弹缓动_指数衰减_In
      cur = _EasingType_Rebound_Damping_In(pro, start, end);
      break;
    case EasingType_Rebound_Damping_Out://反弹缓动_指数衰减_Out
      cur = _EasingType_Rebound_Damping_Out(pro, start, end);
      break;
    case EasingType_Rebound_Damping_InOut://反弹缓动_指数衰减_InOut
      cur = _EasingType_Rebound_Damping_InOut(pro, start, end);
      break;
    case EasingType_Cube_Spill_In://三次方缓动_溢出_In
      cur = _EasingType_Cube_Spill_In(pro, start, end);
      break;
    case EasingType_Cube_Spill_Out://三次方缓动_溢出_Out
      cur = _EasingType_Cube_Spill_Out(pro, start, end);
      break;
    case EasingType_Cube_Spill_InOut://三次方缓动_溢出_InOut
      cur = _EasingType_Cube_Spill_InOut(pro, start, end);
      break;
    case EasingType_Sinusoid_Damping_In://正弦曲线缓动_指数衰减_In
      cur = _EasingType_Sinusoid_Damping_In(pro, start, end);
      break;
    case EasingType_Sinusoid_Damping_Out://正弦曲线缓动_指数衰减_Out
      cur = _EasingType_Sinusoid_Damping_Out(pro, start, end);
      break;
    case EasingType_Sinusoid_Damping_InOut://正弦曲线缓动_指数衰减_InOut
      cur = _EasingType_Sinusoid_Damping_InOut(pro, start, end);
      break;
    default:
      return false;
  }
  return true;
}

void Easing_Woking(int uType, int dwTimer, int dwSleep, void (*lpCbk)(double, double, int, int, bool &a, int, void *),
                   int param1, void *param2, double start, double end, int addr) {
  int count;
  double pro;
  double cur;
  bool bStop = false;
  int i = 0;

  if (addr != 0) {
  }//

  count = dwTimer / dwSleep;

  for (int c = 0; c < count; c++) {
    i += 1;
    osDelay(dwSleep);
    pro = i * (double) dwSleep / (double) dwTimer;
    if (Easing_GetProgress(uType, start, end, pro, cur) == false) { pro = 1; }
    if (lpCbk != nullptr) { (*lpCbk)(pro, cur, count, i, bStop, param1, param2); }
    if (pro == 1 || bStop) { break; }
  }
}

//==========easing===================
void Easing_Strat(
    int Type,      //运动类型
    int AllTime,    //运动总时间
    int intervalTime,  //运动间隔时间
    double start,    //缓动开始值
    double end,      //缓动结束值
    void (*lpCbk)(double, double, int, int, bool &a, int, void *),      //回调指针
    bool bThread,    //是否线程模式
    int param1,      //附加参数1
    void *param2      //附加参数2
) {
  Easing_Woking(Type, AllTime, intervalTime, lpCbk, param1, param2, start, end, 0);
};