#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QPen>

class ComparatorItem : public ComponentItem
{
    Q_OBJECT

public:
    ComparatorItem(Component *component);

public slots:
    void updateOutputs();
};

class Comparator : public Component
{
    Q_OBJECT

public:
    Comparator(QObject *parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
