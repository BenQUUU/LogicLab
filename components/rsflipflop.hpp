#pragma once

#include "../core/component.hpp"

#include <QGraphicsPathItem>
#include <QPen>

class RSFlipFlopItem : public ComponentItem
{
    Q_OBJECT

public:
    RSFlipFlopItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _Q {false};
    bool _notQ {true};
};

class RSFlipFlop : public Component
{
    Q_OBJECT

public:
    RSFlipFlop(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
