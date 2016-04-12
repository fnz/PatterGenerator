#include "FullscreenWindow.h"

static const double openDuration = 0.0;
static const double closeDuration = 0.0;

using namespace cocos2d;
using namespace cocos2d::ui;

bool FullscreenWindow::initWindow() {
    windowSize = Director::getInstance()->getWinSize();
    return true;
}

void FullscreenWindow::open() {
    setOpacity(255);
    setVisible(true);
}

void FullscreenWindow::close() {
    setOpacity(0);
    setVisible(false);
}

double FullscreenWindow::getOpenDuration() {
    return openDuration;
}

double FullscreenWindow::getCloseDuration() {
    return closeDuration;
}

bool FullscreenWindow::isFullscreen() {
    return true;
}
