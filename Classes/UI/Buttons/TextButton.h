#pragma once

#include "ButtonBase.h"

namespace cocos2d {
namespace ui {
class Scale9Sprite;
}
}

class TextButton : public ButtonBase {
public:
    static TextButton* create(const std::string& text, const int width = 0);
    static TextButton* create(const std::string& text, const std::string& font, int fontSize, const cocos2d::Color3B& color, const int width = 0);

    bool initWithParams(const std::string& text, const std::string& font, int fontSize, const cocos2d::Color3B& color, const int width);

    virtual void setRegularColor(const cocos2d::Color3B& color) override;
    virtual void setEnabled(bool value) override;
    virtual void setOpacity(GLubyte opacity) override;
    virtual void setCascadeOpacityEnabled(bool value) override;
    virtual void onEnter() override;

protected:
    virtual void push() override;
};
