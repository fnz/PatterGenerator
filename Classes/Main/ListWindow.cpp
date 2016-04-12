#include "ListWindow.h"

#include "UIConstants.h"
#include "Scroll.h"
#include "TextButton.h"
#include "SaveManager.h"
#include "WindowManager.h"
#include "ConfirmationWindow.h"
#include "IconButton.h"
#include "Generator.h"

#include "AndroidCpp.h"

using namespace cocos2d;

bool ListWindow::initWindow(const std::function<void(const std::string&, const std::string&)>& callback) {
    PopupWindow::initWindow(Size(MED(850), MED(500)));
    ListWindow::callback = callback;

    auto description = Label::createWithTTF("Select a pattern from the list below.", UIConstants::textFont, UIConstants::textFontSize);
    description->setColor(UIConstants::textColorSecondary);
    description->setAnchorPoint(Vec2(0.0, 1.0));
    description->setPosition(Vec2(MED(-400), MED(230)));
    addChild(description);

    scroll = Scroll::create(Size(MED(800), MED(405)));
    scroll->setInertiaScrollEnabled(true);
    scroll->setScrollBarColor(UIConstants::primaryColorDark);
    scroll->setAnchorPoint(Vec2(0.5, 1.0));
    scroll->setPosition(Vec2(0, MED(195)));
    scroll->setScrollBarEnabled(false);
    addChild(scroll);

    auto patterns = SaveManager::getInstance()->get();

    float height = 0.0;
    for (int i = 0; i < patterns.size(); i++) {
        auto str = "0. RRRRRRRRRRRRRRRR / HHHHHHHHHHHHHHHH";
        auto button = TextButton::create(str, UIConstants::buttonFont, MED(30), UIConstants::textColorDark);
        auto buttonLabel = button->getLabel();
        buttonLabel->setAnchorPoint(Vec2(0.0, 0.5));
        buttonLabel->setPosition(buttonLabel->getPosition() - Vec2(0.5 * buttonLabel->getBoundingBox().size.width, 0));
        button->setName("button");
        scroll->pushTop(button, 0, TextHAlignment::LEFT);

        height += button->getBoundingBox().size.height;
        auto pos = scroll->convertToInterfaceCoords(button->getPosition());
        pos.x = scroll->getContentSize().width - MED(15);
        pos.y -= 0.5 * button->getBoundingBox().size.height;

        auto deleteButton = IconButton::create("cancel.png");
        auto deleteCallback = [this, button, deleteButton]() {
			WindowManager::getInstance()->show<ConfirmationWindow>("Are you sure you want to delete this pattern?", [this, button, deleteButton](bool answer) {
				if (answer) {
					SaveManager::getInstance()->remove(button->getTag());
					scroll->removeNode(button);
					deleteButton->removeFromParent();
					refresh();
				}
			});
        };
        deleteButton->addCallback(deleteCallback);
        deleteButton->setAnchorPoint(Vec2(1.0, 0.5));
        deleteButton->setPosition(pos);
        deleteButton->setScale(0.5);
        scroll->addChild(deleteButton);
    }

    refresh();

    if (height > scroll->getContentSize().height) {
        description->setString("Select a pattern from the scrollable list below.");
    }

    return true;
}

void ListWindow::refresh() {
    std::vector<TextButton*> buttons;
    for (const auto& child : scroll->getChildren()) {
        if (child->getName() == "button") {
            auto button = dynamic_cast<TextButton*>(child);
            if (button != nullptr) {
                buttons.push_back(button);
            }
        }
    }

    if (buttons.empty()) {
        WindowManager::getInstance()->closeWindow(this);
    }

    std::sort(std::begin(buttons), std::end(buttons), [](const TextButton* a, const TextButton* b) -> bool {
		return a->getPosition().y > b->getPosition().y;
    });

    auto patterns = SaveManager::getInstance()->get();

    for (int i = 0; i < patterns.size(); i++) {
        auto button = buttons[i];
        //auto str = std::to_string(i + 1) + ". " + patterns[i].first + " / " + patterns[i].second;
		auto str = std::to_string(i + 1) + ". " + Generator::convertToDisplay(patterns[i].first);
		button->getLabel()->setString(str);
        button->setTag(i);
        auto pattern = patterns[i].first;
        auto orchestration = patterns[i].second;
        button->addCallback([this, pattern, orchestration]() {
			callback(pattern, orchestration);
			WindowManager::getInstance()->closeWindow(this);
        });
    }
}
