#pragma once

#include "../core/component.hpp"

#include <QGraphicsPathItem>
#include <QPen>

class NorGateItem : public ComponentItem
{
    Q_OBJECT

public:
    NorGateItem(Component* component);

public slots:
    void updateOutputs();
};

class NorGate : public Component
{
    Q_OBJECT

public:
    NorGate(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
