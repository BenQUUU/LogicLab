#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class DFlipFlopItem : public ComponentItem
{
    Q_OBJECT

public:
    DFlipFlopItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _Q {false};
    bool _notQ {true};
};

class DFlipFlop : public Component
{
    Q_OBJECT

public:
    DFlipFlop(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
