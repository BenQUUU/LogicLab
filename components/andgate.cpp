#include "andgate.hpp"

#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

#include <QPen>

AndGateItem::AndGateItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.moveTo(20, 0);
    path.lineTo(20, 30);
    path.arcTo(20, 0, 30, 30, -90, 180);
    path.closeSubpath();

    path.moveTo(5, 5);
    path.lineTo(20, 5);

    path.moveTo(5, 25);
    path.lineTo(20, 25);

    path.moveTo(50, 15);
    path.lineTo(65, 15);

    setPath(path);

    _inputs << new InputPin(this, 0, 0, 270);
    _inputs << new InputPin(this, 0, 20, 270);

    _outputs << new OutputPin(this, 60, 10, 90);

    updateOutputs();
}

void AndGateItem::updateOutputs()
{
    _outputs[0]->setValue(_inputs[0]->value() & _inputs[1]->value());
}

AndGate::AndGate(QObject *parent) :
    Component(parent)
{
}

QString AndGate::category() const
{
    return "Logic gates";
}

QString AndGate::name() const
{
    return "AND gate";
}

ComponentItem *AndGate::item()
{
    return new AndGateItem(this);
}
