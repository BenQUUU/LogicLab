#pragma once

#include "../core/component.hpp"

#include <QGraphicsPathItem>
#include <QPen>

class AndGateItem : public ComponentItem
{
    Q_OBJECT

public:
    AndGateItem(Component *component);

public slots:
    void updateOutputs();
};

class AndGate : public Component
{
    Q_OBJECT

public:
    AndGate(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
