#pragma once

#include "PopupWindow.h"
#include <functional>

class ConfirmationWindow : public PopupWindow {
public:
    bool initWindow(const std::string& message, const std::function<void(bool)>& callback);
    virtual void onBack() override;

private:
    std::function<void(bool)> callback;
};