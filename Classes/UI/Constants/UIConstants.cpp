#include "UIConstants.h"

using namespace cocos2d;

cocos2d::Color3B UIConstants::primaryColorBlack;
cocos2d::Color3B UIConstants::primaryColorDark;
cocos2d::Color3B UIConstants::primaryColor;
cocos2d::Color3B UIConstants::primaryColorLight;
cocos2d::Color3B UIConstants::primaryColorWhite;
cocos2d::Color3B UIConstants::accentColor;
cocos2d::Color3B UIConstants::textColorDark;
cocos2d::Color3B UIConstants::textColorLight;
cocos2d::Color3B UIConstants::textColorSecondary;

std::string UIConstants::textFont;
int UIConstants::textFontSize;

std::string UIConstants::buttonFont;
int UIConstants::buttonFontSize;

std::string UIConstants::patternFont;
int UIConstants::patternFontSize;

void UIConstants::init() {
    primaryColorBlack = Color3B(0x21, 0x21, 0x21);
    primaryColorDark = Color3B(0x45, 0x5A, 0x64);
    primaryColor = Color3B(0x60, 0x7D, 0x8B);
    primaryColorLight = Color3B(0xCF, 0xD8, 0xDC);
    primaryColorWhite = Color3B(0xFF, 0xFF, 0xFF);
    accentColor = Color3B(0xFF, 0x57, 0x22);
    textColorDark = Color3B(0x21, 0x21, 0x21);
    textColorLight = Color3B(0xFF, 0xFF, 0xFF);
    textColorSecondary = Color3B(0x72, 0x72, 0x72);

    textFont = "fonts/Roboto-Bold.ttf";
    textFontSize = MED(22);

    buttonFont = "fonts/Roboto-Black.ttf";
    buttonFontSize = MED(20);

    patternFont = "fonts/Roboto-Black.ttf";
    patternFontSize = MED(80);
}