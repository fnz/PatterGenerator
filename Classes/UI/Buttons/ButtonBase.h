#pragma once

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "Synthesize.h"

class ButtonBase : public cocos2d::ui::Button {
public:
    ButtonBase();
    virtual bool init() override;

    void addCallback(const std::function<void()>& callback);
    void addLabel(const std::string& text);
    void addLabel(const std::string& text, const std::string& font, int size, const cocos2d::Color3B& color);
    void addIcon(const std::string& iconName);
    void addIcon(const std::string& iconName, const cocos2d::Color3B& color);

    SYNTHESIZE_READONLY(std::function<void()>, callback, Callback);
    SYNTHESIZE_READONLY(cocos2d::Label*, label, Label);
    SYNTHESIZE_READONLY(cocos2d::Sprite*, icon, Icon);
    SYNTHESIZE(float, pushDuration, PushDuration);
    SYNTHESIZE(float, callbackDelay, CallbackDelay);

    virtual void setEnabled(bool value) override;
    virtual void setRegularColor(const cocos2d::Color3B& color);
    virtual void setPressedColor(const cocos2d::Color3B& color);
    virtual void setInactiveColor(const cocos2d::Color3B& color);

    virtual void setOpacity(GLubyte opacity) override;
    virtual void addChild(Node* child, int localZOrder = 0, const std::string& name = "") override;
    virtual void removeAllChildren() override;

protected:
    cocos2d::Color3B regularColor;
    cocos2d::Color3B pressedColor;
    cocos2d::Color3B inactiveColor;

    virtual void arrange();
    virtual void push();
    virtual void eventListener(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToNormal() override;
};