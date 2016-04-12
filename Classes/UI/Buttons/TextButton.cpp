#include "TextButton.h"

#include "UIConstants.h"
#include "ui/UIScale9Sprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#pragma mark - Init

TextButton* TextButton::create(const std::string& text, const int width) {
    return TextButton::create(text, UIConstants::buttonFont, UIConstants::buttonFontSize, UIConstants::textColorDark, width);
}

TextButton* TextButton::create(const std::string& text, const std::string& font, int fontSize, const cocos2d::Color3B& color, const int width) {
    auto ret = new (std::nothrow) TextButton();
    if (ret && ret->initWithParams(text, font, fontSize, color, width)) {
        return ret;
    } else {
        delete ret;
        return nullptr;
    }
}

bool TextButton::initWithParams(const std::string& text, const std::string& font, int fontSize, const cocos2d::Color3B& color, const int width) {
    Button::init("box_transparent.png", "", "", Widget::TextureResType::PLIST);
    ButtonBase::init();
    setScale9Enabled(true);
    GameUtils::setCascade(_buttonNormalRenderer, true);

    addLabel(text, font, fontSize, color);
    if (width >= 0) {
        label->setWidth(2.0 * width);
    }
    auto buttonSize = label->getBoundingBox().size;
    buttonSize.width += MED(20);
    buttonSize.height += MED(20);
    setContentSize(buttonSize);
    label->setPosition(0.5 * getContentSize());

    setRegularColor(UIConstants::textColorDark);
    setPressedColor(UIConstants::accentColor);
    setInactiveColor(UIConstants::primaryColorDark);

    return true;
}

#pragma mark - Actions

void TextButton::push() {
    if (!isEnabled()) {
        return;
    }
    label->stopAllActions();
    label->setColor(pressedColor);
    label->runAction(TintTo::create(pushDuration, regularColor));
}

void TextButton::setEnabled(bool value) {
    ButtonBase::setEnabled(value);
    label->setColor(value ? regularColor : inactiveColor);
    label->setOpacity(value ? 255 : 100);
}

void TextButton::setOpacity(GLubyte opacity) {
    if (label) {
        label->setOpacity(opacity);
    }
}

void TextButton::setCascadeOpacityEnabled(bool value) {
    return;
}

void TextButton::onEnter() {
    ButtonBase::onEnter();
    label->setColor(regularColor);
}

void TextButton::setRegularColor(const cocos2d::Color3B& color) {
    regularColor = color;
    label->setColor(color);
}