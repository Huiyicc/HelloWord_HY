//
// Created by 19254 on 2023/9/3.
//

#ifndef HELLOWORD_DYNAMIC_FW_UTILS_HPP
#define HELLOWORD_DYNAMIC_FW_UTILS_HPP

namespace HYSDK::Debug {
void Print(const char *str);
}

#define Println(format, ...) \
char _ss_[64];               \
memset(_ss_, ' ', 64);       \
sprintf(_ss_,format,__VA_ARGS__); \
HYSDK::Debug::Print(_ss_)

#endif //HELLOWORD_DYNAMIC_FW_UTILS_HPP
