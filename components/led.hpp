#pragma once

#include "../core/component.hpp"

class LEDItem : public ComponentItem
{
    Q_OBJECT

public:
    LEDItem(Component* component);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

class LED : public Component
{
    Q_OBJECT

public:
    LED(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
