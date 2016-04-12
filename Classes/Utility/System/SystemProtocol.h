#pragma once

#include "BaseObserverProtocol.h"

class SystemProtocol : public BaseObserverProtocol {
public:
    virtual void applicationDidEnterBackground() {
    }
    virtual void applicationWillEnterForeground() {
    }
};