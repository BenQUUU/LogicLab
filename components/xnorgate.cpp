#include "xnorgate.hpp"

#include "../core/inputpin.hpp"
#include "../core/outputpin.hpp"

XnorGateItem::XnorGateItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.moveTo(20, 0);
    path.quadTo(30, 15, 20, 30);
    path.quadTo(45, 30, 50, 15);
    path.quadTo(45, 0, 20, 0);

    path.moveTo(15, 1);
    path.quadTo(25, 15, 15, 29);
    path.quadTo(25, 15, 15, 1);

    path.addEllipse(50, 12.5, 6, 6);

    path.moveTo(5, 5);
    path.lineTo(23, 5);

    path.moveTo(5, 25);
    path.lineTo(23, 25);

    path.moveTo(55, 15);
    path.lineTo(65, 15);

    setPath(path);

    _inputs << new InputPin(this, 0, 0, 270);
    _inputs << new InputPin(this, 0, 20, 270);

    _outputs << new OutputPin(this, 60, 10, 90);

    updateOutputs();
}

void XnorGateItem::updateOutputs()
{
    _outputs[0]->setValue(!(_inputs[0]->value() ^ _inputs[1]->value()));
}

XnorGate::XnorGate(QObject *parent) :
    Component(parent)
{ }

QString XnorGate::category() const
{
    return "LOGIC GATES";
}

QString XnorGate::name() const
{
    return "XNOR GATE";
}

ComponentItem *XnorGate::item()
{
    return new XnorGateItem(this);
}
