#include "button.hpp"

#include "../core/outputpin.hpp"

ButtonItem::ButtonItem(Component *component) :
    ComponentItem(component),
    _mouseDown(false)
{
    QPainterPath path;
    path.addRect(0, 0, 50, 50);
    path.moveTo(50, 25);
    path.lineTo(65, 25);
    setPath(path);

    _outputs << new OutputPin(this, 60, 20, 90);
}

void ButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ComponentItem::paint(painter, option, widget);

    QLinearGradient gradient(0, 5, 0, 45);
    gradient.setSpread(QGradient::RepeatSpread);

    gradient.setColorAt(0, QColor(240, 240, 240));
    gradient.setColorAt(1, QColor(220, 220, 220));

    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawEllipse(QPoint(25, 25), 12, 12);

    if (_mouseDown) {
        gradient.setColorAt(0, QColor(140, 200, 10));
        gradient.setColorAt(1, QColor(125, 180, 15));
        painter->setPen(QPen(QColor(125, 170, 20), 1.0));
    } else {
        gradient.setColorAt(0, Qt::white);
        gradient.setColorAt(1, QColor(230, 230, 230));
        painter->setPen(QPen(QColor(220, 220, 220), 1.0));
    }

    painter->setBrush(gradient);
    painter->drawEllipse(QPoint(25, 25), 12, 12);
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    const QRectF button(5, 5, 30, 30);
    if (button.contains(event->pos())) {
        _mouseDown = true;
        _outputs[0]->setValue(true);
        setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    ComponentItem::mousePressEvent(event);
}

void ButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ComponentItem::mouseReleaseEvent(event);

    if (!_mouseDown) return;

    _mouseDown = false;
    _outputs[0]->setValue(false);
    setFlag(QGraphicsItem::ItemIsMovable);
}

Button::Button(QObject *parent) :
    Component(parent)
{ }

QString Button::category() const
{
    return "INPUTS";
}

QString Button::name() const
{
    return "BUTTON";
}

ComponentItem *Button::item()
{
    return new ButtonItem(this);
}
