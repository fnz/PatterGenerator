#pragma once

#include <string>

#include "Window.h"
#include "cocos2d.h"

class FullscreenWindow : public Window {
public:
    virtual bool initWindow();
    virtual void open() override;
    virtual void close() override;
    virtual double getOpenDuration() override;
    virtual double getCloseDuration() override;
    virtual bool isFullscreen() override;
};