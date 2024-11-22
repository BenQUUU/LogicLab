#pragma once

#include "../core/component.hpp"

class LogicalLowItem : public ComponentItem
{
    Q_OBJECT

public:
    LogicalLowItem(Component* component);
};

class LogicalLow : public Component
{
    Q_OBJECT

public:
    LogicalLow(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
