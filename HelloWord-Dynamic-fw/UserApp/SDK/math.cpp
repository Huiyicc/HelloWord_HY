//
// Created by 19254 on 2024/2/27.
//
#include "math.hpp"
#include <cmath>

namespace HYSDK {

// ����ʵ��
Point calculateCirclePoint(double x0, double y0, double radius, double angle_in_radians) {
  // ���ݼ�����ת��Ϊֱ������ϵ��ʽ
  return {x0 + radius * cos(angle_in_radians), y0 + radius * sin(angle_in_radians)};
}

} // namespace HYSDK