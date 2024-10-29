#include "outputpin.hpp"

#include "wire.hpp"

#include <QBrush>

OutputPin::OutputPin(ComponentItem* parent, double x, double y, int angle) :
    PinItem(parent, x, y, angle),
    _value(false)
{ }

OutputPin::~OutputPin()
{
    qDeleteAll(_wires);
    _wires.clear();
}

void OutputPin::addWire(Wire *wire)
{
    _wires << wire;
}

void OutputPin::removeWire(Wire *wire)
{
    _wires.removeAll(wire);
}

void OutputPin::updateWires()
{
    for(Wire *wire : _wires)
    {
        wire->updatePath();
    }
}

void OutputPin::setValue(bool value)
{
    if(_value == value)
    {
        return;
    }

    _value = value;

    setBrush(colorForValue(_value));
    update();

    emit valueChanged(_value);
}
