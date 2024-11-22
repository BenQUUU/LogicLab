#pragma once

#include "../core/component.hpp"

class LogicalHighItem : public ComponentItem
{
    Q_OBJECT

public:
    LogicalHighItem(Component* component);
};

class LogicalHigh : public Component
{
    Q_OBJECT

public:
    LogicalHigh(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
