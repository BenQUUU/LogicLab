#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class FullAdderItem : public ComponentItem
{
    Q_OBJECT

public:
    FullAdderItem(Component *component);

public slots:
    void updateOutputs();
};

class FullAdder : public Component
{
    Q_OBJECT

public:
    FullAdder(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
