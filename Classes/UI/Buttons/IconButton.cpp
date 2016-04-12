#include "IconButton.h"

#include "UIConstants.h"
#include "ui/UIScale9Sprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#pragma mark - Init

IconButton* IconButton::create(const std::string& iconName) {
    auto ret = new (std::nothrow) IconButton();
    if (ret && ret->initWithParams(iconName)) {
        return ret;
    } else {
        delete ret;
        return nullptr;
    }
}

bool IconButton::initWithParams(const std::string& iconName) {
    Button::init("box_transparent.png", "", "", Widget::TextureResType::PLIST);
    ButtonBase::init();
    setScale9Enabled(true);
    GameUtils::setCascade(_buttonNormalRenderer, true);

    addIcon(iconName);
    auto buttonSize = icon->getBoundingBox().size;
    buttonSize.width += MED(20);
    buttonSize.height += MED(20);
    setContentSize(buttonSize);
    icon->setPosition(0.5 * getContentSize());

    setRegularColor(UIConstants::primaryColorBlack);
    setPressedColor(UIConstants::accentColor);
    setInactiveColor(UIConstants::primaryColorBlack);

    return true;
}

#pragma mark - Actions

void IconButton::push() {
    if (!isEnabled()) {
        return;
    }
    icon->stopAllActions();
    icon->setColor(pressedColor);
    icon->runAction(TintTo::create(pushDuration, regularColor));
}

void IconButton::setEnabled(bool value) {
    ButtonBase::setEnabled(value);
    icon->setColor(value ? regularColor : inactiveColor);
    icon->setOpacity(value ? 255 : 100);
}

void IconButton::setOpacity(GLubyte opacity) {
    if (icon) {
        icon->setOpacity(opacity);
    }
}

void IconButton::setCascadeOpacityEnabled(bool value) {
    return;
}

void IconButton::onEnter() {
    ButtonBase::onEnter();
    icon->setColor(regularColor);
}

void IconButton::setRegularColor(const cocos2d::Color3B& color) {
    regularColor = color;
    icon->setColor(color);
}