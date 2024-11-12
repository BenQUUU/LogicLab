#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class TFlipFlopItem : public ComponentItem
{
    Q_OBJECT

public:
    TFlipFlopItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _Q {false};
    bool _notQ {true};
};

class TFlipFlop : public Component
{
    Q_OBJECT

public:
    TFlipFlop(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
