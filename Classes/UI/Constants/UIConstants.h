#pragma once

#include "cocos2d.h"
#include "GameUtils.h"

#define BIG_MED_LOW(__BIG__, __MED__, __LOW__) (GameUtils::getDeviceType() == 2 ? (__BIG__) : (__MED__))
#define MED(__VAL__) BIG_MED_LOW((__VAL__)*2, (__VAL__), (__VAL__) / 2)

class UIConstants {
public:
    static void init();

    static cocos2d::Color3B primaryColorBlack;
    static cocos2d::Color3B primaryColorDark;
    static cocos2d::Color3B primaryColor;
    static cocos2d::Color3B primaryColorLight;
    static cocos2d::Color3B primaryColorWhite;
    static cocos2d::Color3B accentColor;
    static cocos2d::Color3B textColorDark;
    static cocos2d::Color3B textColorLight;
    static cocos2d::Color3B textColorSecondary;

    static std::string textFont;
    static int textFontSize;

    static std::string buttonFont;
    static int buttonFontSize;

    static std::string patternFont;
    static int patternFontSize;
};