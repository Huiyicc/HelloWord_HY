//
// Created by 19254 on 2023/9/3.
//

#ifndef HELLOWORD_DYNAMIC_FW_UTILS_HPP
#define HELLOWORD_DYNAMIC_FW_UTILS_HPP
#include <cstring>
#include <cstdio>

namespace HYSDK::Debug {
void Print(unsigned char reportID,const char *str);
}

#define REPORTID_IO 4

#define Println(format, ...) \
{char _ss_[64];               \
memset(_ss_, ' ', 64);       \
sprintf(_ss_, format, ##__VA_ARGS__); \
HYSDK::Debug::Print(REPORTID_IO, _ss_);};

#endif //HELLOWORD_DYNAMIC_FW_UTILS_HPP
