#pragma once

#include "../core/component.hpp"

#include <QGraphicsPathItem>
#include <QPen>

class SRFlipFlopItem : public ComponentItem
{
    Q_OBJECT

public:
    SRFlipFlopItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _Q;
    bool _notQ;
};

class SRFlipFlop : public Component
{
    Q_OBJECT

public:
    SRFlipFlop(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
