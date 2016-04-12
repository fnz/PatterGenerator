#include "ConfirmationWindow.h"

#include "WindowManager.h"
#include "UIConstants.h"
#include "TextButton.h"

using namespace cocos2d;

bool ConfirmationWindow::initWindow(const std::string& message, const std::function<void(bool)>& callback) {
    ConfirmationWindow::callback = callback;
    auto windowWidth = MED(500);

    auto text = Label::createWithTTF(message, UIConstants::textFont, 2.0 * UIConstants::textFontSize);
    text->setScale(0.5);
    text->setWidth(2.0 * (windowWidth - MED(50)));

    auto textHeight = text->getBoundingBox().size.height;
    auto windowHeight = textHeight + MED(100);

    PopupWindow::initWindow(Size(windowWidth, windowHeight));

    text->setAnchorPoint(Vec2(0.5, 1.0));
    text->setPosition(Vec2(0.0, 0.5 * windowHeight - MED(20)));
    text->setColor(UIConstants::textColorSecondary);
    addChild(text);

    auto yes = TextButton::create("YES");
    yes->setPosition(Vec2(0.5 * windowWidth - MED(60), -0.5 * windowHeight + MED(30)));
    yes->addCallback([this, callback]() {
		WindowManager::getInstance()->closeWindow(this);
		callback(true);
    });
    addChild(yes);

    auto no = TextButton::create("NO");
    no->setPosition(Vec2(0.5 * windowWidth - MED(140), -0.5 * windowHeight + MED(30)));
    no->addCallback([this, callback]() {
		WindowManager::getInstance()->closeWindow(this);
		callback(false);
    });
    addChild(no);

    return true;
}

void ConfirmationWindow::onBack() {
    callback(false);
    WindowManager::getInstance()->closeWindow(this);
}