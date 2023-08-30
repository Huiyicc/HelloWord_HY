//
// Created by 19254 on 2023/8/28.
//
#include "strings.hpp"

namespace SDK {
    int VerbatimSplit(const std::string &str, std::vector <std::string> &out) {
        out.clear();
        int n = 0;
        auto c = str.size();
        int z = 0;
        std::vector<char> strss(str.begin(), str.end());
        while (n < c) {
            if (n + 2 >= c) {
                z = 1;
            } else {
                if (str[n] > 128) {
                    z = 3;
                } else if (str[n] != 13) {
                    z = 1;
                } else {
                    z = ((str[n + 1] == 10) ? 2 : 1);
                }
            }
            if (n + z > c) {
                z = c - n;
            }
            out.push_back(str.substr(n, z) + '\0'+'\0');
            n += z;
        }
        return out.size();
    }
}