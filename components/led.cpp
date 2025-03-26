#include "led.hpp"

#include "../core/inputpin.hpp"

LEDItem::LEDItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 50, 50);
    path.moveTo(0, 25);
    path.lineTo(-20, 25);
    setPath(path);

    _inputs << new InputPin(this, -20, 20, 90);
}

void LEDItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ComponentItem::paint(painter, option, widget);

    painter->setPen(Qt::NoPen);

    if (_inputs[0]->value()) {
        painter->setBrush(Qt::red);
    } else {
        QLinearGradient gradient(0, 5, 0, 45);
        gradient.setSpread(QGradient::RepeatSpread);
        gradient.setColorAt(0, Qt::white);
        gradient.setColorAt(1, QColor(230, 230, 230));
        painter->setBrush(gradient);
        painter->setPen(QPen(QColor(220, 220, 220), 1.0));
    }

    painter->drawEllipse(QPoint(25, 25), 20, 20);
}

LED::LED(QObject* parent) :
    Component(parent)
{
}

QString LED::category() const
{
    return "OUTPUTS";
}

QString LED::name() const
{
    return "LED";
}

ComponentItem* LED::item()
{
    return new LEDItem(this);
}
