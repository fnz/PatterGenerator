#include "Window.h"

#include "WindowManager.h"

#include <string>
#include <vector>

USING_NS_CC;

const std::string Window::windowElementNodeName = "window_element_node";

Window::Window()
: alert(false)
, closeOnOuterClick(true)
, state(WindowState::Unknown) {
}

void Window::onOpen() {
}

void Window::onClose() {
}

void Window::onBack() {
    WindowManager::getInstance()->closeWindow(this);
}

void Window::markWindowElements() {
    const auto& children = getChildren();
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->setName(windowElementNodeName);
    }
}

void Window::clearWindow() {
    const auto& children = getChildren();
    std::vector<Node*> childrenToDelete;
    for (int i = 0; i < children.size(); i++) {
        auto child = children.at(i);
        if (child->getName() != windowElementNodeName) {
            childrenToDelete.push_back(child);
        }
    }

    for (auto child : childrenToDelete) {
        child->removeFromParentAndCleanup(true);
    }
}
