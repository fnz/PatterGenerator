#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "AndroidCpp.h"

int std::stoi(const std::string& string) {
    return stonum<int>(string);
}

long std::stol(const std::string& string) {
    return stonum<long>(string);
}

#endif
