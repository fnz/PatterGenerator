#pragma once

#include <iosfwd>
#include <set>

#include "cocos2d.h"
#include "ui/UIScrollView.h"

namespace cocos2d {
class Size;
}

class Scroll : public cocos2d::ui::ScrollView {
public:
    enum class Orientation {
        Positive, // ось Y направлена вниз
        Negative // ось Y направлена вверх
    };

    static Scroll* createBase();
    static Scroll* create(const cocos2d::Size& size); // создать настроенный для добавления в окно скролл
    static Scroll* createHorizontal(const cocos2d::Size& size);

    Scroll();

    virtual ~Scroll();

    void setOrientation(Orientation orientation);

    //all of these functions are related to innerContainer.
    virtual void addChild(Node* child) override;
    virtual void addChild(Node* child, int localZOrder) override;
    virtual void addChild(Node* child, int zOrder, int tag) override;
    virtual void addChild(Node* child, int zOrder, const std::string& name) override;

    // добавляет ноду в горизонтальный скролл с правого края
    void pushRight(Node* node, float horizIndent = 0, cocos2d::TextVAlignment alignment = cocos2d::TextVAlignment::CENTER);

    // добавляет ноду ниже topY
    // horizIndent - используется только для RIGHT && LEFT выравнивания
    void pushTop(Node* node, float verticalIndent = 0, cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::CENTER, float horizIndent = 0);

    // убирает ноду со скролла, обновляя значения topY и innerContentSize
    void removeNode(Node* node);

    // Вставить ноду перед заданной нодой
    void insertNode(Node* beforeNode, Node* node, float verticalIndent = 0, cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::CENTER, float horizIndent = 0);

    // очищает скролл
    void clear();

    // Вернет реальные координаты ноды на скролле по координатам, указанным при вызове addChild()
    cocos2d::Vec2 convertToRealCoords(const cocos2d::Vec2& interfaceCoords) const;

    // Вернет координаты ноды, которые нужно указать в addChild чтобы ноды добавилсь с заданными реальными координатами
    cocos2d::Vec2 convertToInterfaceCoords(const cocos2d::Vec2& realCoords) const;

    float topY; //позиция по Y для добавления следующего элемента
    float curWidth; // текущая ширина содержимого горизонтального скрола

    virtual void setVisible(bool value) override;

protected:
    void prepareToAddChild(Node* child);
    Orientation orientation;
};
