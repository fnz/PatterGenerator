#include "MessageWindow.h"

#include "WindowManager.h"
#include "UIConstants.h"
#include "TextButton.h"

using namespace cocos2d;

bool MessageWindow::initWindow(const std::string& message, const std::function<void()>& callback) {
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

    auto ok = TextButton::create("OK");
    ok->setPosition(Vec2(0, -0.5 * windowHeight + MED(30)));
    ok->addCallback([this, callback]() {
		if (callback) {
			callback();
		}
		WindowManager::getInstance()->closeWindow(this);
    });
    addChild(ok);

    return true;
}