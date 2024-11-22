#include "logicalhigh.hpp"

#include "../core/outputpin.hpp"

LogicalHighItem::LogicalHighItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 30, 30);
    path.moveTo(30, 15);
    path.lineTo(45, 15);
    setPath(path);

    QGraphicsTextItem *label = new QGraphicsTextItem("1", this);
    label->setPos(7, 3);
    label->setDefaultTextColor(Qt::black);

    _outputs << new OutputPin(this, 40, 10, 90);

    _outputs[0]->setValue(true);
}

LogicalHigh::LogicalHigh(QObject *parent) :
    Component(parent)
{
}

QString LogicalHigh::category() const
{
    return "INPUTS";
}

QString LogicalHigh::name() const
{
    return "HIGH";
}

ComponentItem *LogicalHigh::item()
{
    return new LogicalHighItem(this);
}
