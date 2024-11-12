#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class MultiplexerItem : public ComponentItem
{
    Q_OBJECT

public:
    MultiplexerItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _output {false};
    bool _enabled {true};
};

class Multiplexer : public Component
{
    Q_OBJECT

public:
    Multiplexer(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
