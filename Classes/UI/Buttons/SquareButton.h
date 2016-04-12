#pragma once

#include "ButtonBase.h"

namespace cocos2d {
namespace ui {
class Scale9Sprite;
}
}

class SquareButton : public ButtonBase {
public:
    static SquareButton* create(const cocos2d::Size& size);
    static SquareButton* create(const cocos2d::Size& size, const cocos2d::Color3B& color);
    bool initWithParams(const cocos2d::Size& size, const cocos2d::Color3B& color);

    virtual void setEnabled(bool value) override;
    virtual void onEnter() override;

protected:
    virtual void push() override;
    cocos2d::ui::Scale9Sprite* shadow;
};
