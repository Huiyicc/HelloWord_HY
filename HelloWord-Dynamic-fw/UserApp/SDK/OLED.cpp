//
// Created by 19254 on 2023/8/28.
//
#include "OLED.hpp"
#include "os_define.hpp"

namespace SDK {
    void OLEDDrawUTF8Str_wqy12_STL(int x, int y, const std::string &str, int autoDraw) {
        std::vector <std::string> oledText;
        SDK::VerbatimSplit(str, oledText);
        g_sysCtx->Device.oled->SetFont(font_default);
        auto ry = y;
        for (auto &i: oledText) {
            g_sysCtx->Device.oled->DrawUTF8(x, ry, i.c_str());
            ry += 14;
        }
    }

    void OLEDDrawUTF8Str_wqy12(int x, int y, const std::vector<std::string>&str,int colortype){
        g_sysCtx->Device.oled->SetFont(font_default);
        auto ry = y;
        for (auto &i: str) {
            g_sysCtx->Device.oled->DrawUTF8(x, ry, i.c_str());
            ry += 14;
        }
    }
}