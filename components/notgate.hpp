#pragma once

#include "../core/component.hpp"

#include <QGraphicsPathItem>
#include <QPen>

class NotGateItem : public ComponentItem
{
    Q_OBJECT

public:
    NotGateItem(Component* component);

public slots:
    void updateOutputs();
};

class NotGate : public Component
{
    Q_OBJECT

public:
    NotGate(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
