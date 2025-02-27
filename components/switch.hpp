#pragma once

#include "../core/component.hpp"

class SwitchItem : public ComponentItem
{
    Q_OBJECT

public:
    SwitchItem(Component* component);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    bool _mouseDown;
};

class Switch : public Component
{
    Q_OBJECT

public:
    Switch(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
