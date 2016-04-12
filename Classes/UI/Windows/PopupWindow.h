#pragma once

#include <string>

#include "Window.h"
#include "cocos2d.h"

class PopupWindow : public Window {
public:
    virtual bool initWindow(const cocos2d::Size& size);
    virtual void open() override;
    virtual void close() override;
    virtual double getOpenDuration() override;
    virtual double getCloseDuration() override;
    virtual bool isFullscreen() override;

protected:
    cocos2d::Sprite* header = nullptr;
};
