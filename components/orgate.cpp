#include "orgate.hpp"

#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

OrGateItem::OrGateItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.moveTo(20, 0);
    path.quadTo(30, 15, 20, 30);
    path.quadTo(45, 30, 50, 15);
    path.quadTo(45, 0, 20, 0);

    path.moveTo(5, 5);
    path.lineTo(23, 5);

    path.moveTo(5, 25);
    path.lineTo(23, 25);

    path.moveTo(50, 15);
    path.lineTo(65, 15);

    setPath(path);

    _inputs << new InputPin(this, 0, 0, 270);
    _inputs << new InputPin(this, 0, 20, 270);

    _outputs << new OutputPin(this, 60, 10, 90);

    updateOutputs();
}

void OrGateItem::updateOutputs()
{
    _outputs[0]->setValue(_inputs[0]->value() | _inputs[1]->value());
}

OrGate::OrGate(QObject *parent) :
    Component(parent)
{
}

QString OrGate::category() const
{
    return "LOGIC GATES";
}

QString OrGate::name() const
{
    return "OR GATE";
}

ComponentItem *OrGate::item()
{
    return new OrGateItem(this);
}
