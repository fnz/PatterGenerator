#include "SquareButton.h"

#include "UIConstants.h"
#include "ui/UIScale9Sprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#pragma mark - Init

SquareButton* SquareButton::create(const Size& size) {
    return create(size, UIConstants::primaryColorLight);
}

SquareButton* SquareButton::create(const Size& size, const Color3B& color) {
    auto ret = new (std::nothrow) SquareButton();
    if (ret && ret->initWithParams(size, color)) {
        return ret;
    } else {
        delete ret;
        return nullptr;
    }
}

bool SquareButton::initWithParams(const Size& size, const Color3B& color) {
    Button::init("box.png", "", "", Widget::TextureResType::PLIST);
    ButtonBase::init();
    setScale9Enabled(true);
    setContentSize(size);

    shadow = Scale9Sprite::createWithSpriteFrameName("shadow.png");
    shadow->setContentSize(size + Size(MED(8), MED(8)));
    shadow->setPosition(0.5 * size - Vec2(0, MED(3)));
    shadow->setOpacity(120);
    addChild(shadow, -1);

    setRegularColor(color);
    int shift = 64;
    setPressedColor(Color3B(std::max(0, regularColor.r - shift),
                            std::max(0, regularColor.g - shift),
                            std::max(0, regularColor.b - shift)));
    setInactiveColor(color);

    return true;
}

#pragma mark - Actions

void SquareButton::push() {
    if (!isEnabled()) {
        return;
    }
    stopAllActions();
    setColor(regularColor);
    runAction(Sequence::create(TintTo::create(0.5 * pushDuration, pressedColor),
                               TintTo::create(0.5 * pushDuration, regularColor),
                               nullptr));

    shadow->stopAllActions();
    shadow->setScale(1.0);
    auto size = getContentSize();
    float scaleX = (size.width + MED(6)) / size.width;
    float scaleY = (size.height + MED(6)) / size.height;
    shadow->runAction(Sequence::create(ScaleTo::create(0.5 * pushDuration, scaleX, scaleY),
                                       ScaleTo::create(0.5 * pushDuration, 1.0),
                                       nullptr));

    shadow->runAction(Sequence::create(FadeTo::create(0.5 * pushDuration, 100),
                                       FadeTo::create(0.5 * pushDuration, 120),
                                       nullptr));
}

void SquareButton::setEnabled(bool value) {
    ButtonBase::setEnabled(value);
    auto size = getContentSize();
    shadow->setPosition(0.5 * size - Vec2(0, MED(value ? 3 : 1)));
    auto shadowSize = MED(value ? 8 : 4);
    shadow->setContentSize(size + Size(shadowSize, shadowSize));
    setColor(value ? regularColor : inactiveColor);
    if (label) {
        label->setOpacity(value ? 255 : 100);
    }
}

void SquareButton::onEnter() {
    ButtonBase::onEnter();
    setColor(isEnabled() ? regularColor : inactiveColor);
}