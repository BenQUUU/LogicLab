#include "logicallow.hpp"

#include "../core/outputpin.hpp"

LogicalLowItem::LogicalLowItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 30, 30);
    path.moveTo(30, 15);
    path.lineTo(45, 15);
    setPath(path);

    QGraphicsTextItem *label = new QGraphicsTextItem("0", this);
    label->setPos(7, 3);
    label->setDefaultTextColor(Qt::black);

    _outputs << new OutputPin(this, 40, 10, 90);

    _outputs[0]->setValue(false);
}

LogicalLow::LogicalLow(QObject *parent) :
    Component(parent)
{ }

QString LogicalLow::category() const
{
    return "INPUTS";
}

QString LogicalLow::name() const
{
    return "LOW";
}

ComponentItem *LogicalLow::item()
{
    return new LogicalLowItem(this);
}
