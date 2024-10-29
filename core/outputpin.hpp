#pragma once

#include "pinitem.hpp"

class Wire;

class OutputPin : public PinItem
{
    Q_OBJECT

public:
    OutputPin(ComponentItem* parent, double x, double y, int angle = -1);
    ~OutputPin();

    enum { Type = QGraphicsItem::UserType + 1 };
    inline int type() const { return Type; }

    inline QVector<Wire*> getWires() const { return _wires; }
    void addWire(Wire* wire);
    void removeWire(Wire* wire);
    void updateWires();

    inline bool getValue() const { return _value; }
    void setValue(bool value);

signals:
    void valueChanged(bool value);

private:
    bool _value;
    QVector<Wire*> _wires;
};
