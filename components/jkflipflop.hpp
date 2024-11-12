#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class JKFlipFlopItem : public ComponentItem
{
    Q_OBJECT

public:
    JKFlipFlopItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _Q {false};
    bool _notQ {true};
};

class JKFlipFlop : public Component
{
    Q_OBJECT

public:
    JKFlipFlop(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
