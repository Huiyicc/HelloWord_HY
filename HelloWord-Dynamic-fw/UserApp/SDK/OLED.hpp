//
// Created by 19254 on 2023/8/28.
//

#ifndef HELLOWORD_DYNAMIC_FW_OLED_HPP
#define HELLOWORD_DYNAMIC_FW_OLED_HPP
#include "strings.hpp"

namespace SDK {
    void OLEDDrawUTF8Str_wqy12_STL(int x, int y, const std::string &str, int autoDraw);
    void OLEDDrawUTF8Str_wqy12(int x=0, int y=0, const std::vector<std::string>&str={""},int colortype=1);
}

#endif //HELLOWORD_DYNAMIC_FW_OLED_HPP
