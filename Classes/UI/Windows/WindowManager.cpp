#include "WindowManager.h"

#include <assert.h>
#include <algorithm>
#include <functional>
#include <iterator>

#include "ObserverManager.h"
#include "Window.h"
#include "WindowManagerProtocol.h"
#include "cocos2d.h"

USING_NS_CC;

const int blackOpacity = 150;

#pragma mark - Ctor and Dtor

WindowManager::WindowManager()
: rootNode(nullptr)
, alertNode(nullptr)
, normalBlack(nullptr)
, alertBlack(nullptr)
, alertWindow(nullptr) {
}

WindowManager::~WindowManager() {
}

#pragma mark - Parent node

// Этот метод должен быть вызван строго после того, как загрузятся текстуры
void WindowManager::setParentNode(cocos2d::Node* parentNode, int zWindows, int zAlerts) {
    if (rootNode != nullptr) {
        rootNode->removeFromParentAndCleanup(true);
    }
    if (alertNode != nullptr) {
        alertNode->removeFromParentAndCleanup(true);
    }

    // Родительская нода для обычных окон
    rootNode = Node::create();
    rootNode->setPosition(0.5 * Director::getInstance()->getWinSize());
    parentNode->addChild(rootNode, zWindows);

    normalBlack = createBlack(rootNode, false);

    // Родительская нода для алертов
    alertNode = Node::create();
    alertNode->setPosition(0.5 * Director::getInstance()->getWinSize());
    parentNode->addChild(alertNode, zAlerts);

    alertBlack = createBlack(alertNode, true);
}

cocos2d::Sprite* WindowManager::createBlack(Node* node, bool alert) {
    auto black = Sprite::createWithSpriteFrameName("shadow.png");
    auto screenSize = Director::getInstance()->getWinSize();
    black->setScaleX(2 * screenSize.width / black->getContentSize().width);
    black->setScaleY(2 * screenSize.height / black->getContentSize().height);
    black->setOpacity(0);
    black->setVisible(false);
    node->addChild(black);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this, black](Touch* touch, Event* event) -> bool {
        return (black->getOpacity() != 0);
    };

    if (!alert) {
        touchListener->onTouchEnded = [this](Touch* touch, Event* event) {
            auto topWindow = getTopWindow();
            if (topWindow == nullptr) {
                return;
            }
            auto relativeLocation = topWindow->convertToNodeSpace(touch->getLocation());
            bool windowContainsTouch = false;
            for (auto child : topWindow->getChildren()) {
                if (child->getBoundingBox().containsPoint(relativeLocation)) {
                    windowContainsTouch = true;
                }
            }
            if (!windowContainsTouch && topWindow->getCloseOnOuterClick()) {
                closeWindow(topWindow);
            }
            return;
        };
    }

    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, black);

    auto backListener = EventListenerKeyboard::create();
    backListener->onKeyReleased = [this, black](EventKeyboard::KeyCode keyCode, Event* event) {
        auto topWindow = getTopWindow();
		if (topWindow == nullptr || topWindow->state != WindowState::Normal) {
            return;
        }
		
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
			topWindow->onBack();
        }
        event->stopPropagation();
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backListener, black);

    return black;
}

#pragma mark - Core

void WindowManager::openWindow(Window* window) {
    assert(rootNode != nullptr);
    assert(alertNode != nullptr);
    assert(window != nullptr);

    if (!windows.empty()) {
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(windows.back(), true);
    }

    int topZ;
    Node* parentNode;
    Sprite* black;
    if (window->getAlert()) {
        topZ = 2;
        parentNode = alertNode;
        black = alertBlack;
        if (alertWindow != nullptr) {
            closeWindow(alertWindow);
        }
        alertWindow = window;
    } else {
        topZ = getTopZ() + 2;
        parentNode = rootNode;
        black = normalBlack;
        windows.push_back(window);
    }

    parentNode->addChild(window, topZ);
    window->setOpacity(0);

    black->stopAllActions();
    black->setVisible(true);
    if (black->getOpacity() != blackOpacity) {
        black->runAction(FadeTo::create(window->getOpenDuration(), blackOpacity));
    }
    parentNode->reorderChild(black, topZ - 1);

    window->state = WindowState::Opening;
    ObserverManager::notify(&WindowManagerProtocol::windowWillOpen, window);
    double duration = window->getOpenDuration();
    auto disableListeners = [this, black, window]() {
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(window, true);
		Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(black, true);
    };
    auto callback = [this, window, black]() {
        window->state = WindowState::Normal;
        window->onOpen();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(window, true);
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(black, true);
        ObserverManager::notify(&WindowManagerProtocol::windowOpened, window);
    };
    /* после того как клик обрабатывается, новые ноды добавляются в пул слушателей и эти слушатели автоматически включаются.
        текущий код выполнится до того, как клик закончит обрабатываться, и слушатели тачей у открываемого окна будут активны,
        кнопки будут нажиматься. поэтому через eps по времени, вызовется метод, который их отключит на время открытия. */
    parentNode->runAction(Sequence::create(DelayTime::create(0.001 * duration),
                                           CallFunc::create(disableListeners),
                                           DelayTime::create(0.999 * duration),
                                           CallFunc::create(callback), nullptr));
    window->open();
}

void WindowManager::closeWindow(Window* window) {
    assert(window != nullptr);
    if (window->getAlert()) {
        alertWindow = nullptr;
    } else {
        assert(std::find(std::begin(windows), std::end(windows), window) != std::end(windows));
    }

    auto topWindow = getTopWindow();
    if (topWindow->getState() != WindowState::Normal) {
        closingWindows.push(window);
        return;
    }

    auto black = window->getAlert() ? alertBlack : normalBlack;

    window->state = WindowState::Closing;
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(window, true);
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(black, true);
    ObserverManager::notify(&WindowManagerProtocol::windowWillClose, window);

    auto closeFinishedCallback = [this, window, black]() {
        if (!window->getAlert()) {
            auto it = std::remove(windows.begin(), windows.end(), window);
            windows.erase(it, windows.end());
        }
        if (!windows.empty()) {
            Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(windows.back(), true);
            Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(black, true);
        }
        ObserverManager::notify(&WindowManagerProtocol::windowClosed, window);
		checkQueues();
    };

    float closeDuration = window->getCloseDuration();
    window->runAction(Sequence::create(DelayTime::create(closeDuration),
                                       CallFunc::create(closeFinishedCallback),
                                       RemoveSelf::create(),
                                       nullptr));

    // если мы закрываем текущее окно - урбать черную подложку, иначе переместить под топовое окно
    auto blackFadeOutAction = Sequence::create(FadeTo::create(closeDuration, 0),
                                               ToggleVisibility::create(),
                                               nullptr);

    if (window->getAlert()) {
        alertBlack->runAction(blackFadeOutAction);
    } else {
        if (getTopZ() == 0) {
            normalBlack->runAction(blackFadeOutAction);
        } else {
            auto penult = getPenultWindow();
            int z = getTopZ() - 3;
            if (penult != nullptr && penult->isFullscreen()) {
                auto reorder = [this, z]() {
					rootNode->reorderChild(normalBlack, z);
                };
                normalBlack->runAction(Sequence::create(blackFadeOutAction, CallFunc::create(reorder), nullptr));
            } else {
                rootNode->reorderChild(normalBlack, z);
            }
        }
    }

    window->close();
    window->onClose();
}

void WindowManager::closeAllWindows() {
    double maxCloseDuration = 0.0;

    auto allWindows = windows;
    if (alertWindow != nullptr) {
        allWindows.push_back(alertWindow);
    }

    for (auto window : allWindows) {
        window->state = WindowState::Closing;
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(window, true);
        ObserverManager::notify(&WindowManagerProtocol::windowWillClose, window);
        auto closeFinishedCallback = [window, this]() {
            if (!window->getAlert()) {
                auto it = std::remove(windows.begin(), windows.end(), window);
                windows.erase(it, windows.end());
            }
            ObserverManager::notify(&WindowManagerProtocol::windowClosed, window);
        };

        float duration = window->getCloseDuration();
        if (duration > maxCloseDuration) {
            maxCloseDuration = duration;
        }
        window->runAction(Sequence::create(DelayTime::create(duration),
                                           CallFunc::create(closeFinishedCallback),
                                           RemoveSelf::create(),
                                           nullptr));
        window->onClose();
        window->close();
    }

    auto blackFadeOutAction = Sequence::create(FadeTo::create(maxCloseDuration, 0),
                                               ToggleVisibility::create(),
                                               nullptr);
    normalBlack->runAction(blackFadeOutAction);
    alertBlack->runAction(blackFadeOutAction->clone());
}

#pragma mark - Aux

Window* WindowManager::getTopWindow(bool includingAlert) {
    if (includingAlert && alertWindow != nullptr && alertWindow->state == WindowState::Normal) {
        return alertWindow;
    }
    if (!windows.empty()) {
        return windows[windows.size() - 1];
    }
    return nullptr;
}

Window* WindowManager::getPenultWindow() {
    if (windows.size() < 2) {
        return nullptr;
    } else {
        return windows[windows.size() - 2];
    }
}

int WindowManager::getTopZ() {
    auto window = getTopWindow();
    if (window == nullptr) {
        return 0;
    } else {
        return window->getLocalZOrder();
    }
}

void WindowManager::checkQueues() {
    if (!closingWindows.empty()) {
        auto window = closingWindows.front();
        closeWindow(window);
        closingWindows.pop();
    }
}
