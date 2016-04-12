#pragma once

#include "PopupWindow.h"
#include <functional>
#include <string>

class Scroll;

class ListWindow : public PopupWindow {
public:
    bool initWindow(const std::function<void(const std::string&, const std::string&)>& callback);

private:
    Scroll* scroll = nullptr;
    void refresh();
    std::function<void(const std::string&, const std::string&)> callback;
};