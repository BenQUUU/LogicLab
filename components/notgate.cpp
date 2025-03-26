#include "notgate.hpp"

#include "../core/inputpin.hpp"
#include "../core/outputpin.hpp"

NotGateItem::NotGateItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.moveTo(20, 0);
    path.lineTo(20, 30);
    path.lineTo(50, 15);
    path.lineTo(20, 0);

    path.addEllipse(50, 12.5, 6, 6);

    path.moveTo(5, 15);
    path.lineTo(20, 15);

    path.moveTo(55, 15);
    path.lineTo(65, 15);

    setPath(path);

    _inputs << new InputPin(this, 0, 10, 270);

    _outputs << new OutputPin(this, 60, 10, 90);

    updateOutputs();
}

void NotGateItem::updateOutputs()
{
    _outputs[0]->setValue(!(_inputs[0]->value()));
}

NotGate::NotGate(QObject *parent) :
    Component(parent)
{ }

QString NotGate::category() const
{
    return "LOGIC GATES";
}

QString NotGate::name() const
{
    return "NOT GATE";
}

ComponentItem *NotGate::item()
{
    return new NotGateItem(this);
}
