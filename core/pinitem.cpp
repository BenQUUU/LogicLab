#include "pinitem.hpp"

#include "componentitem.hpp"

#include <QPen>
#include <QGraphicsSceneMouseEvent>

QColor PinItem::colorForValue(bool value)
{
    if(value) {
        return QColor(Qt::green);
    }
    else {
        return QColor(Qt::white);
    }
}

PinItem::PinItem(ComponentItem* parent, double x, double y, int angle) :
    QObject(parent),
    QGraphicsEllipseItem(parent),
    _angle(angle)
{
    setPen(QPen((Qt::black), 1));
    setBrush(Qt::white);
    setRect(0.0, 0.0, 10.0, 10.0);
    setPos(x, y);

    setAcceptHoverEvents(true);
}

void PinItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setRect(-2.0, -2.0, 14.0, 14.0);
}

void PinItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setRect(0.0, 0.0, 10.0, 10.0);
}

void PinItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}
