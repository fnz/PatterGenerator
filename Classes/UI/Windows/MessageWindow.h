#pragma once

#include "PopupWindow.h"
#include <functional>

class MessageWindow : public PopupWindow {
public:
    bool initWindow(const std::string& message, const std::function<void()>& callback = std::function<void()>());
};