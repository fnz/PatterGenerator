#pragma once

#include <vector>
#include <queue>

#include "cocos2d.h"
#include "Singleton.h"
#include "Synthesize.h"
#include "GameUtils.h"

class Window;
namespace cocos2d {
class Node;
class Sprite;
}

class WindowManager : public Singleton<WindowManager> {
    friend class Singleton<WindowManager>;

public:
    void setParentNode(cocos2d::Node* parentNode, int zWindows, int zAlert);

    template <bool alert, typename WindowType, typename... ArgumentTypes>
    void show(ArgumentTypes... args) {
        WindowType* window = new WindowType();
        if (!window->initWindow(args...)) {
            CCLOG("Init method returned false, aborting");
            return;
        }
        window->autorelease();
        window->setAlert(alert);
        GameUtils::setCascade(window, true);
        this->openWindow(window);
    }

    template <typename WindowType, typename... ArgumentTypes>
    void show(ArgumentTypes... args) {
        show<false, WindowType, ArgumentTypes...>(args...);
    }

    template <typename WindowType, typename... ArgumentTypes>
    void showAlert(ArgumentTypes... args) {
        show<true, WindowType, ArgumentTypes...>(args...);
    }

    template <typename WindowType>
    bool containsWindow() const {
        for (auto window : windows) {
            if (dynamic_cast<WindowType*>(window) != nullptr) {
                return true;
            }
        }
        return false;
    }

    void openWindow(Window* window);
    void closeWindow(Window* window);
    void closeAllWindows();

    Window* getTopWindow(bool includingAlert = false);
    Window* getPenultWindow();
    int getTopZ();
    SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Window*>, windows, Windows);

    ~WindowManager();

private:
    WindowManager();

    cocos2d::Node* rootNode;
    cocos2d::Node* alertNode;
    cocos2d::Sprite* createBlack(cocos2d::Node* node, bool alert);
    cocos2d::Sprite* normalBlack;
    cocos2d::Sprite* alertBlack;

    Window* alertWindow;

    std::queue<Window*> closingWindows;
    std::queue<Window*> openingWindows;
    void checkQueues();
};
