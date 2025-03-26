#include "inputpin.hpp"

#include "componentitem.hpp"
#include "outputpin.hpp"
#include "wire.hpp"

#include <QBrush>

InputPin::InputPin(ComponentItem *parent, double x, double y, int angle) :
    PinItem(parent, x, y, angle),
    _wire(0)
{
    connect(this, SIGNAL(valueChanged(bool)), parent, SLOT(updateOutputs()));
    connect(this, SIGNAL(valueChanged(bool)), parent, SLOT(update()));

    connect(this, SIGNAL(valueChanged(bool)), SLOT(updateColor(bool)));
}

InputPin::~InputPin()
{
    if (!_wire) return;

    delete _wire;
    _wire = nullptr;
}

void InputPin::setWire(Wire *wire)
{
    _wire = wire;

    emit valueChanged(value());
}

void InputPin::updateWires()
{
    if (!_wire) return;

    _wire->updatePath();
}

bool InputPin::value()
{
    if (!_wire) return false;

    return _wire->getValue();
}

void InputPin::updateColor(bool value)
{
    setBrush(colorForValue(value));
}


