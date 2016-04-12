#include "PopupWindow.h"

#include "UIConstants.h"
#include "WindowManager.h"
#include "ui/UIScale9Sprite.h"

static const double openDuration = 0.2;
static const double closeDuration = 0.1;

using namespace cocos2d;
using namespace cocos2d::ui;

bool PopupWindow::initWindow(const cocos2d::Size& size) {
    windowSize = size;

    auto back = Scale9Sprite::createWithSpriteFrameName("box.png");
    back->setColor(UIConstants::primaryColorWhite);
    back->setContentSize(size);
    addChild(back);

    auto shadow = Scale9Sprite::createWithSpriteFrameName("shadow.png");
    shadow->setContentSize(size + Size(MED(12), MED(12)));
    shadow->setPosition(-Vec2(0, MED(3)));
    shadow->setOpacity(120);
    addChild(shadow, -1);

    return true;
}

void PopupWindow::open() {
    setOpacity(0);
    setScale(0.7);

    runAction(Sequence::create(DelayTime::create(0.5 * openDuration),
                               FadeTo::create(openDuration, 255),
                               nullptr));

    runAction(Sequence::create(DelayTime::create(0.5 * openDuration),
                               EaseOut::create(ScaleTo::create(0.4 * openDuration, 1.05), 2.0),
                               EaseOut::create(ScaleTo::create(0.1 * openDuration, 1.0), 2.0),
                               nullptr));
}

void PopupWindow::close() {
    runAction(FadeTo::create(closeDuration, 0));
    runAction(EaseIn::create(ScaleTo::create(closeDuration, 0.7), 2.0));
}

double PopupWindow::getOpenDuration() {
    return openDuration;
}

double PopupWindow::getCloseDuration() {
    return closeDuration;
}

bool PopupWindow::isFullscreen() {
    return false;
}
