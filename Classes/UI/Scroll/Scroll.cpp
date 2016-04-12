#include "Scroll.h"

#include <limits.h>
#include <cmath>
#include <iostream>
#include <new>

#include "ui/UILayout.h"

USING_NS_CC;

Scroll* Scroll::createBase() {
    Scroll* scroll = new (std::nothrow) Scroll();
    if (scroll && scroll->init()) {
        scroll->autorelease();
        scroll->setCascadeOpacityEnabled(true);
        scroll->setPropagateTouchEvents(false);
        return scroll;
    }
    CC_SAFE_DELETE(scroll);
    return nullptr;
}

Scroll* Scroll::createHorizontal(const cocos2d::Size& size) {
    auto scroll = createBase();
    if (scroll) {
        scroll->setBackGroundColorType(Layout::BackGroundColorType::NONE);
        scroll->setDirection(ScrollView::Direction::HORIZONTAL);
        scroll->setContentSize(size);
    }
    return scroll;
}

Scroll* Scroll::create(const Size& size) {
    auto scroll = createBase();
    if (scroll) {
        scroll->setBackGroundColorType(Layout::BackGroundColorType::NONE);
        scroll->setDirection(ScrollView::Direction::VERTICAL);
        scroll->setOrientation(Scroll::Orientation::Negative);
        scroll->setContentSize(size);
        scroll->setAnchorPoint(Vec2(0.5, 0.5));
    }
    return scroll;
}

Scroll::Scroll()
: topY(0)
, curWidth(0)
, orientation(Orientation::Positive) {
}

Scroll::~Scroll() {
}

void Scroll::setOrientation(Orientation orientation) {
    Scroll::orientation = orientation;
}

void Scroll::addChild(Node* child) {
    Scroll::addChild(child, child->getLocalZOrder(), child->getTag());
}

void Scroll::addChild(Node* child, int localZOrder) {
    Scroll::addChild(child, localZOrder, child->getTag());
}

void Scroll::addChild(Node* child, int zOrder, int tag) {
    prepareToAddChild(child);
    ScrollView::addChild(child, zOrder, tag);
}

void Scroll::addChild(Node* child, int zOrder, const std::string& name) {
    prepareToAddChild(child);
    ScrollView::addChild(child, zOrder, name);
}

void Scroll::prepareToAddChild(Node* child) {
    Vec2 childPos = child->getPosition();
    const Size& innerContainerSize = getInnerContainerSize();
    if (_direction == Direction::HORIZONTAL || _direction == Direction::BOTH) {
        if (innerContainerSize.width < childPos.x) {
            setInnerContainerSize(Size(childPos.x + child->getBoundingBox().size.width * (1.0f - child->getAnchorPoint().x), innerContainerSize.height));
        }
    }
    if (_direction == Direction::VERTICAL || _direction == Direction::BOTH) {
        int sign = orientation == Scroll::Orientation::Negative ? -1 : 1;
        float childBottomPointY = sign * childPos.y + child->getBoundingBox().size.height * child->getAnchorPoint().y;
        if (innerContainerSize.height < childBottomPointY) {
            Size originalInnerSize = getInnerContainerSize();
            setInnerContainerSize(Size(innerContainerSize.width, childBottomPointY));
            Size newInnerSize = getInnerContainerSize();
            float offsetY = originalInnerSize.height - newInnerSize.height;
            auto& children = _innerContainer->getChildren();
            for (auto& child : children) {
                child->setPosition(Vec2(child->getPosition().x, child->getPosition().y - offsetY));
            }
        }

        if (orientation == Orientation::Negative) {
            childPos.y = -childPos.y;
        }
        childPos.y = getInnerContainerSize().height - childPos.y;
        child->setPosition(childPos);
    }
}

Vec2 Scroll::convertToRealCoords(const cocos2d::Vec2& interfaceCoords) const {
    Vec2 realCoords = interfaceCoords;

    if (orientation == Orientation::Negative) {
        realCoords.y = -realCoords.y;
    }
    realCoords.y = getInnerContainerSize().height - realCoords.y;

    return realCoords;
}

Vec2 Scroll::convertToInterfaceCoords(const cocos2d::Vec2& realCoords) const {
    Vec2 interfaceCoords = realCoords;

    interfaceCoords.y = getInnerContainerSize().height - realCoords.y;
    if (orientation == Orientation::Negative) {
        interfaceCoords.y = -interfaceCoords.y;
    }

    return interfaceCoords;
}

void Scroll::pushRight(Node* node, float horizIndent, TextVAlignment alignment) {
    curWidth += horizIndent;

    float yAnchor;
    float y;
    switch (alignment) {
        case TextVAlignment::TOP:
            yAnchor = 1;
            y = getContentSize().height;
            break;

        default:
        case TextVAlignment::CENTER:
            yAnchor = 0.5;
            y = getContentSize().height / 2;
            break;

        case TextVAlignment::BOTTOM:
            yAnchor = 0;
            y = 0;
            break;
    }
    node->setAnchorPoint(Vec2(0, yAnchor));
    node->setPosition(Vec2(curWidth, y));
    this->addChild(node);
    curWidth += node->getBoundingBox().size.width;
}

void Scroll::pushTop(Node* node, float verticalIndent, cocos2d::TextHAlignment alignment, float horizIndent) {
    float orientationKoef = orientation == Orientation::Positive ? -1 : 1;
    topY -= verticalIndent * orientationKoef;

    float xAnchor;
    float x;
    switch (alignment) {
        case TextHAlignment::LEFT:
            xAnchor = 0;
            x = horizIndent;
            break;

        default:
        case TextHAlignment::CENTER:
            xAnchor = 0.5;
            x = getBoundingBox().size.width / 2;
            break;

        case TextHAlignment::RIGHT:
            xAnchor = 1;
            x = getBoundingBox().size.width - horizIndent;
            break;
    }
    node->setAnchorPoint(Vec2(xAnchor, 1));
    node->setPosition(Vec2(x, topY));
    this->addChild(node);
    topY -= node->getBoundingBox().size.height * orientationKoef;
}

void Scroll::removeNode(Node* node) {
    auto height = node->getBoundingBox().size.height;
    auto size = getInnerContainerSize();
    size.height -= height;
    topY -= height;
    auto dy = Vec2(0.0, height);
    auto children = getChildren();
    if (size.height > getContentSize().height) {
        for (const auto& child : children) {
            if (child->getPosition().y > node->getPosition().y) {
                child->setPosition(child->getPosition() - dy);
            }
        }
        setInnerContainerSize(size);
    } else {
        for (const auto& child : children) {
            if (child->getPosition().y < node->getPosition().y) {
                child->setPosition(child->getPosition() + dy);
            }
        }
        for (const auto& child : children) {
            child->setPosition(child->getPosition() + Vec2(0.0, getContentSize().height - getInnerContainerSize().height));
        }
        setInnerContainerSize(getContentSize());
    }

    node->removeFromParentAndCleanup(true);
}

void Scroll::insertNode(Node* beforeNode, Node* node, float verticalIndent, cocos2d::TextHAlignment alignment, float horizIndent) {
    float height = node->getBoundingBox().size.height;
    auto size = getInnerContainerSize();
    size.height += height;
    topY += (height + verticalIndent);
    auto dy = Vec2(0.0, height + verticalIndent);
    auto children = getChildren();
    if (size.height > getContentSize().height) {
        for (const auto& child : children) {
            if (child->getPosition().y > beforeNode->getPosition().y) {
                child->setPosition(child->getPosition() + dy);
            }
        }
        setInnerContainerSize(size);
    } else {
        for (const auto& child : children) {
            if (child->getPosition().y < beforeNode->getPosition().y) {
                child->setPosition(child->getPosition() - dy);
            }
        }
        for (const auto& child : children) {
            child->setPosition(child->getPosition() + Vec2(0.0, getContentSize().height - getInnerContainerSize().height));
        }
        setInnerContainerSize(getContentSize());
    }

    float xAnchor;
    float x;
    switch (alignment) {
        case TextHAlignment::LEFT:
            xAnchor = 0;
            x = horizIndent;
            break;

        default:
        case TextHAlignment::CENTER:
            xAnchor = 0.5;
            x = getBoundingBox().size.width / 2;
            break;

        case TextHAlignment::RIGHT:
            xAnchor = 1;
            x = getBoundingBox().size.width - horizIndent;
            break;
    }
    node->setAnchorPoint(Vec2(xAnchor, 1));
    float insertPosY = beforeNode->getPosition().y + (1.0 - beforeNode->getAnchorPoint().y) * beforeNode->getContentSize().height + node->getAnchorPoint().y * node->getContentSize().height + verticalIndent;
    node->setPosition(Vec2(x, insertPosY));
    ScrollView::addChild(node);
}

void Scroll::clear() {
    this->removeAllChildren();
    this->setInnerContainerSize(Size(getInnerContainerSize().width, 0));
    topY = 0;
    curWidth = 0;
}

void Scroll::setVisible(bool value) {
    Node::setVisible(value);
    for (const auto child : getChildren()) {
        child->setVisible(value);
    }
}
