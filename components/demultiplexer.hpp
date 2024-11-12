#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class DemultiplexerItem : public ComponentItem
{
    Q_OBJECT

public:
    DemultiplexerItem(Component *component);

public slots:
    void updateOutputs();

private:
    bool _enabled {true};
    bool _out0 {false};
    bool _out1 {false};
    bool _out2 {false};
    bool _out3 {false};
};

class Demultiplexer : public Component
{
    Q_OBJECT

public:
    Demultiplexer(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
