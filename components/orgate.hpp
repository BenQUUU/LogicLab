#pragma once

#include "../core/component.hpp"

#include <QGraphicsPathItem>
#include <QPen>

class OrGateItem : public ComponentItem
{
    Q_OBJECT

public:
    OrGateItem(Component *component);

public slots:
    void updateOutputs();
};

class OrGate : public Component
{
    Q_OBJECT

public:
    OrGate(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
