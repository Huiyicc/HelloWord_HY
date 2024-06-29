//
// Created by 19254 on 2024/2/27.
//
#include "math.hpp"
#include <cmath>

namespace HYSDK {

// 函数实现
Point calculateCirclePoint(double x0, double y0, double radius, double angle_in_radians) {
  // 根据极坐标转换为直角坐标系公式
  return {x0 + radius * cos(angle_in_radians), y0 + radius * sin(angle_in_radians)};
}

} // namespace HYSDK