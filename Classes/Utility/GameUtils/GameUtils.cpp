#include "GameUtils.h"

#include <string>
#include <iostream>

#include "cocos2d.h"

using namespace cocos2d;

void GameUtils::setCascade(cocos2d::Node* node, bool value) {
    //node->setCascadeColorEnabled(true);
    node->setCascadeOpacityEnabled(value);

    if (node->getChildren().empty()) {
        return;
    }

    for (auto child : node->getChildren()) {
        setCascade(child, value);
    }
}

std::string GameUtils::format(const std::string fmt_str, ...) {
    int final_n, n = int(fmt_str.size() * 2);
    char* formatted = nullptr;
    va_list ap;
    while (1) {
        if (formatted != nullptr)
            delete[] formatted;
        formatted = new char[n];
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n) {
            n += abs(final_n - n + 1);
        } else {
            break;
        }
    }
    std::string ret = formatted;
    delete[] formatted;
    return ret;
}

int GameUtils::getDeviceType() {
    static int deviceType = -1;

    if (deviceType == -1) {
        int maxTextureSize = Configuration::getInstance()->getMaxTextureSize();
        auto winSize = Director::getInstance()->getWinSize();
        int pixels = winSize.width * winSize.height * pow(cocos2d::Director::getInstance()->getContentScaleFactor(), 2);

        if (pixels < 800 * 480 || maxTextureSize < 2048) {
            deviceType = 0;
        } else if (pixels < 1700 * 1080 || maxTextureSize < 4096) {
            deviceType = 1;
        } else {
            deviceType = 2;
        }
    }

    return deviceType;
}
