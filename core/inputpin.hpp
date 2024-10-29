#pragma once

#include "pinitem.hpp"

class OutputPin;
class Wire;

class InputPin : public PinItem
{
    Q_OBJECT;

public:
    InputPin(ComponentItem* parent, double x, double y, int angle = -1);
    ~InputPin();

    enum { Type = QGraphicsItem::UserType + 2 };
    inline int type() const { return Type; }

    inline const Wire* wire() const { return _wire; }
    void setWire(Wire* wire);
    void updateWires();

    bool value();

signals:
    void valueChanged(bool value);

private slots:
    void updateColor(bool value);

private:
    Wire* _wire;
};
