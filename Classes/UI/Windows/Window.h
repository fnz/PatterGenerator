#pragma once

#include <iosfwd>

#include "Synthesize.h"
#include "cocos2d.h"

class Scroll;

enum class WindowState {
    Opening,
    Normal,
    Closing,
    Unknown,
};

class Window : public cocos2d::Node {
    friend class WindowManager;

public:
    Window();
    virtual void open() = 0;
    virtual void close() = 0;
    virtual double getOpenDuration() = 0;
    virtual double getCloseDuration() = 0;

    virtual void onOpen();
    virtual void onClose();
    virtual void onBack();

    virtual bool isFullscreen() = 0;

    SYNTHESIZE(bool, alert, Alert);
    SYNTHESIZE(bool, closeOnOuterClick, CloseOnOuterClick);
    SYNTHESIZE(WindowState, state, State);
    SYNTHESIZE_READONLY(cocos2d::Size, windowSize, WindowSize);

    static const std::string windowElementNodeName;

protected:
    void markWindowElements();
    void clearWindow();
};
