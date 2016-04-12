#include "ButtonBase.h"

#include "UIConstants.h"
#include "ui/UIScale9Sprite.h"

using namespace cocos2d;

ButtonBase::ButtonBase()
: label(nullptr)
, icon(nullptr)
, pushDuration(0.15)
, callbackDelay(0.1) {
}

bool ButtonBase::init() {
    GameUtils::setCascade(_buttonNormalRenderer, true);
    addTouchEventListener(std::bind(&ButtonBase::eventListener, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}

#pragma mark - Callback, label, icon

void ButtonBase::addCallback(const std::function<void()>& callback) {
    ButtonBase::callback = callback;
}

void ButtonBase::addLabel(const std::string& text) {
    addLabel(text, UIConstants::buttonFont, UIConstants::buttonFontSize, UIConstants::textColorDark);
}

void ButtonBase::addLabel(const std::string& text, const std::string& font, int size, const Color3B& color) {
    if (label != nullptr) {
        removeChild(label);
    }
    label = Label::createWithTTF(text, font, 2 * size);
    label->setWidth(2.0 * (getContentSize().width - MED(20)));
    label->setScale(0.5);
    label->setColor(color);
    label->setAlignment(cocos2d::TextHAlignment::CENTER);
    addChild(label);

    arrange();
}

void ButtonBase::addIcon(const std::string& iconName) {
    addIcon(iconName, UIConstants::textColorDark);
}

void ButtonBase::addIcon(const std::string& iconName, const cocos2d::Color3B& color) {
    if (icon != nullptr) {
        removeChild(icon);
    }
    icon = Sprite::createWithSpriteFrameName(iconName);
    icon->setColor(color);
    addChild(icon);

    arrange();
}

void ButtonBase::arrange() {
    if (icon == nullptr && label == nullptr) {
        return;
    }

    if (icon != nullptr && label == nullptr) {
        icon->setPosition(0.5 * getContentSize());
        return;
    }

    if (icon == nullptr && label != nullptr) {
        label->setPosition(0.5 * getContentSize());
        return;
    }

    auto iconWidth = icon->getBoundingBox().size.width;
    auto labelWidth = icon->getBoundingBox().size.width;
    auto x = 0.5 * (iconWidth - labelWidth);
    icon->setPosition(Vec2(0.5 * getContentSize().width + x - 0.5 * iconWidth, 0.5 * getContentSize().height));
    label->setPosition(Vec2(0.5 * getContentSize().width - x + 0.5 * labelWidth, 0.5 * getContentSize().height));
}

#pragma mark - Event listener

void ButtonBase::push() {
}

void ButtonBase::eventListener(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        if (!isEnabled()) {
            return;
        }
        if (getActionByTag(1) != nullptr) {
            return;
        }
        push();
        if (callback) {
            auto callbackAction = Sequence::create(DelayTime::create(callbackDelay),
                                                   CallFunc::create(callback),
                                                   nullptr);
            callbackAction->setTag(1);
            runAction(callbackAction);
        }
    }
}

#pragma mark - Aux

void ButtonBase::setEnabled(bool value) {
    _enabled = value;
}

void ButtonBase::setRegularColor(const cocos2d::Color3B& color) {
    regularColor = color;
    setColor(regularColor);
}

void ButtonBase::setPressedColor(const cocos2d::Color3B& color) {
    pressedColor = color;
}

void ButtonBase::setInactiveColor(const cocos2d::Color3B& color) {
    inactiveColor = color;
}

void ButtonBase::setOpacity(GLubyte opacity) {
    _buttonNormalRenderer->setOpacity(opacity);
}

void ButtonBase::addChild(Node* child, int localZOrder, const std::string& name) {
    _buttonNormalRenderer->addChild(child, localZOrder, name);
}

void ButtonBase::removeAllChildren() {
    _buttonNormalRenderer->removeAllChildren();
}

void ButtonBase::onPressStateChangedToPressed() {
}

void ButtonBase::onPressStateChangedToNormal() {
}