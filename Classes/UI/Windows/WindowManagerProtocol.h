#pragma once

#include "BaseObserverProtocol.h"

class Window;

struct WindowManagerProtocol : public BaseObserverProtocol {
    virtual void windowWillOpen(Window* window){};
    virtual void windowOpened(Window* window){};
    virtual void windowWillClose(Window* window){};
    virtual void windowClosed(Window* window){};
};