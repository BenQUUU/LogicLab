#include "nandgate.hpp"

#include "../core/inputpin.hpp"
#include "../core/outputpin.hpp"

NandGateItem::NandGateItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.moveTo(20, 0);
    path.lineTo(20, 30);
    path.arcTo(20, 0, 30, 30, -90, 180);
    path.closeSubpath();

    path.addEllipse(50, 12.5, 6, 6);

    path.moveTo(5, 5);
    path.lineTo(20, 5);

    path.moveTo(5, 25);
    path.lineTo(20, 25);

    path.moveTo(55, 15);
    path.lineTo(65, 15);

    setPath(path);

    _inputs << new InputPin(this, 0, 0, 270);
    _inputs << new InputPin(this, 0, 20, 270);

    _outputs << new OutputPin(this, 60, 10, 90);

    updateOutputs();
}

void NandGateItem::updateOutputs()
{
    _outputs[0]->setValue(!(_inputs[0]->value() && _inputs[1]->value()));
}

NandGate::NandGate(QObject* parent) :
    Component(parent)
{
}

QString NandGate::category() const
{
    return "LOGIC GATES";
}

QString NandGate::name() const
{
    return "NAND GATE";
}

ComponentItem* NandGate::item()
{
    return new NandGateItem(this);
}
