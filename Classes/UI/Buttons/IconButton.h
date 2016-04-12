#pragma once

#include "ButtonBase.h"

class IconButton : public ButtonBase {
public:
    static IconButton* create(const std::string& iconName);

    bool initWithParams(const std::string& iconName);

    virtual void setRegularColor(const cocos2d::Color3B& color) override;
    virtual void setEnabled(bool value) override;
    virtual void setOpacity(GLubyte opacity) override;
    virtual void setCascadeOpacityEnabled(bool value) override;
    virtual void onEnter() override;

protected:
    virtual void push() override;
};
