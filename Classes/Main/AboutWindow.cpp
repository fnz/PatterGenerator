#include "AboutWindow.h"

#include "ListWindow.h"

#include "UIConstants.h"
#include "Scroll.h"
#include "TextButton.h"
#include "SaveManager.h"
#include "WindowManager.h"
#include "ConfirmationWindow.h"
#include "IconButton.h"

#include "AndroidCpp.h"

using namespace cocos2d;

bool AboutWindow::initWindow() {
    PopupWindow::initWindow(Size(MED(550), MED(250)));

    auto message = "This app was created to help drummers make cool linear fills. If you have any questions or suggestions, feel free to tap the address below and drop me a letter.";
    auto text = Label::createWithTTF(message, UIConstants::textFont, 2.0 * UIConstants::textFontSize);
    text->setWidth(2.0 * (windowSize.width - MED(50)));
    text->setScale(0.5);
    text->setAnchorPoint(Vec2(0.5, 1.0));
    text->setPosition(Vec2(0.0, 0.5 * windowSize.height - MED(20)));
    text->setColor(UIConstants::textColorSecondary);
    addChild(text);

    auto contact = TextButton::create("victor.a.komarov@gmail.com", UIConstants::buttonFont, 1.5 * UIConstants::buttonFontSize, UIConstants::accentColor, 0);
    contact->addCallback([this]() {
		GameUtils::composeLetter("victor.a.komarov@gmail.com", "Pattern Generator");
		WindowManager::getInstance()->closeWindow(this);
    });
    contact->setPosition(Vec2(0.0, -0.5 * windowSize.height + MED(65)));
    addChild(contact);

    return true;
}
