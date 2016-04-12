#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <sstream>

namespace std {
template <typename T>
std::string to_string(T number) {
    std::stringstream stream;
    stream << number;
    return stream.str();
}

template <typename T>
T stonum(const std::string& string) {
    T number;
    std::stringstream stream(string);
    stream >> number;
    return number;
}

int stoi(const std::string& string);

long stol(const std::string& string);
}

#endif
