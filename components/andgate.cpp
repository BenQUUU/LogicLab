#include "andgate.hpp"

#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

#include <QPen>

AndGateItem::AndGateItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.moveTo(20.0, 0.0);
    path.lineTo(20.0, 30.0);
    path.arcTo(20.0, 0.0, 30.0, 30.0, -90.0, 180.0);
    path.closeSubpath();

    path.moveTo(5.0, 5.0);
    path.lineTo(20.0, 5.0);

    path.moveTo(5.0, 25.0);
    path.lineTo(20.0, 25.0);

    path.moveTo(50.0, 15.0);
    path.lineTo(65.0, 15.0);

    setPath(path);

    _inputs << new InputPin(this, 0, 0, 270);
    _inputs << new InputPin(this, 0, 20, 270);

    _outputs << new OutputPin(this, 60, 10, 90);

    updateOutputs();
}

void AndGateItem::updateOutputs()
{
    _outputs[0]->setValue(_inputs[0]->value() && _inputs[1]->value());
}

AndGate::AndGate(QObject *parent) :
    Component(parent)
{
}

QString AndGate::category() const
{
    return "LOGIC GATES";
}

QString AndGate::name() const
{
    return "AND GATE";
}

ComponentItem *AndGate::item()
{
    return new AndGateItem(this);
}
