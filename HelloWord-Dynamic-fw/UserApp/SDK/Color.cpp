//
// Created by 19254 on 2024/1/26.
//

#include "Color.hpp"

namespace HYSDK::Color {

int CombineColors(char r, char g, char b) {
  return (r << 16) | (g << 8) | b;
}


} // HYSDK