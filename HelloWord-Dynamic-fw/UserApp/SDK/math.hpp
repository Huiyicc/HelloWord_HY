//
// Created by 19254 on 2024/2/27.
//

#ifndef HELLOWORD_DYNAMIC_FW_MATH_HPP
#define HELLOWORD_DYNAMIC_FW_MATH_HPP

namespace HYSDK {

// ×ø±êµã
struct Point {
    double x;
    double y;
};

Point calculateCirclePoint(double x0, double y0, double radius, double angle_in_radians);

}

#endif //HELLOWORD_DYNAMIC_FW_MATH_HPP
