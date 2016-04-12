#include "TestWindow.h"

#include "UIConstants.h"
#include "SquareButton.h"
#include "TextButton.h"
#include "IconButton.h"
#include "WindowManager.h"
#include "MessageWindow.h"
#include "ConfirmationWindow.h"

#include <iostream>

#include "gtest/gtest.h"

using namespace cocos2d;
using namespace cocos2d::ui;

bool TestWindow::initWindow() {
	auto pattern = Sprite::create("/Users/vkomarov/Documents/pattern.png");
	addChild(pattern);
	
	return true;
	
    auto screenSize = Director::getInstance()->getWinSize();

    auto backColor = Color3B(69, 90, 100);

    auto back = Sprite::createWithSpriteFrameName("box.png");
    back->setPosition(0.5 * screenSize);
    back->setScale(100.0f);
    back->setColor(backColor);
    addChild(back);

    auto button = SquareButton::create(Size(MED(200), MED(70)));
    button->setPosition(Vec2(MED(-300), MED(100)));
    button->addLabel("BUTTON");
    addChild(button);

    auto buttonDisabled = SquareButton::create(Size(MED(200), MED(70)));
    buttonDisabled->setPosition(Vec2(MED(-300), MED(0)));
    buttonDisabled->addLabel("DISABLED BUTTON");
    buttonDisabled->setEnabled(false);
    addChild(buttonDisabled);

    auto textButton = TextButton::create("ENABLED TEXT BUTTON", MED(200));
    textButton->setPosition(Vec2(0, MED(100)));
    addChild(textButton);

    auto textButtonDisabled = TextButton::create("DISABLED TEXT BUTTON", MED(200));
    textButtonDisabled->setPosition(Vec2(0, 0));
    textButtonDisabled->setEnabled(false);
    addChild(textButtonDisabled);

    auto iconButton = IconButton::create("load.png");
    iconButton->setPosition(Vec2(MED(300), MED(100)));
    addChild(iconButton);

    auto iconButton2 = IconButton::create("save.png");
    iconButton2->setRegularColor(UIConstants::primaryColorLight);
    iconButton2->setPosition(Vec2(MED(400), MED(100)));
    addChild(iconButton2);

    auto iconButtonDisabled = IconButton::create("help.png");
    iconButtonDisabled->setPosition(Vec2(MED(300), MED(0)));
    iconButtonDisabled->setEnabled(false);
    addChild(iconButtonDisabled);

    auto messageButton = TextButton::create("SHOW MESSAGE WINDOW", MED(200));
    messageButton->addCallback([]() {
		WindowManager::getInstance()->show<MessageWindow>("Hello!");
    });
    messageButton->setPosition(Vec2(-300, MED(-100)));
    addChild(messageButton);

    auto confirmationButton = TextButton::create("SHOW CONFIRMATION WINDOW", MED(300));
    confirmationButton->addCallback([]() {
		WindowManager::getInstance()->show<ConfirmationWindow>("Are you sure you want to omnomnom?", [](bool value){
			std::cout << value << std::endl;
		});
    });
    confirmationButton->setPosition(Vec2(-300, MED(-200)));
    addChild(confirmationButton);

    auto testButton = TextButton::create("RUN TESTS", MED(300));
    testButton->addCallback([]() {
		int argc = 0;
		char* argv = nullptr;
		testing::InitGoogleTest(&argc, &argv);
		std::string result = !RUN_ALL_TESTS() ? "Success" : "Fail";
		WindowManager::getInstance()->show<MessageWindow>(result);
    });
    testButton->setPosition(Vec2(300, MED(-200)));
    addChild(testButton);

    return true;
}