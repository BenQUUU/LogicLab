#include "componentitem.hpp"

#include "component.hpp"
#include "inputpin.hpp"
#include "outputpin.hpp"

#include <QPen>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDialog>

ComponentItem::ComponentItem(Component *component) :
    _component(component)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
    setPen(QPen(Qt::black, 2));
    setBrush(Qt::white);
}

QRectF ComponentItem::boundingRect() const
{
    QRectF rect = QGraphicsPathItem::boundingRect();

    for(InputPin* input : _inputs)
    {
        rect |= input->boundingRect().translated(input->pos());
    }

    for(OutputPin* output : _outputs)
    {
        rect |= output->boundingRect().translated(output->pos());
    }

    return rect;
}

QVariant ComponentItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if((change == ItemPositionHasChanged) || (change == ItemRotationHasChanged) || (change == ItemScaleHasChanged)) {
        foreach(InputPin *input, _inputs)
        input->updateWires();

        foreach(OutputPin *output, _outputs)
            output->updateWires();
    } else if(change == QGraphicsItem::ItemSelectedChange) {
        if(value.toBool()) {
            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
            effect->setOffset(0, 0);
            effect->setColor(QColor(130, 180, 10));
            effect->setBlurRadius(15 * scene()->views().first()->transform().m11()); //also known as scale
            setGraphicsEffect(effect);
        } else {
            setGraphicsEffect(0);
        }
    }

    if(change == ItemPositionChange) {
        QPoint newPos = QGraphicsPathItem::itemChange(change, value).toPoint();

        if(scene() && scene()->property("snapToGrid").toBool()) {
            const int gridSize = scene()->property("gridSize").toInt();

            newPos.setX(newPos.x() - (newPos.x() % gridSize));
            newPos.setY(newPos.y() - (newPos.y() % gridSize));
        }

        return newPos;
    }

    return QGraphicsPathItem::itemChange(change, value);
}

void ComponentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPath(path());
}

void ComponentItem::update()
{
    QGraphicsPathItem::update();
}

QDataStream &operator<<(QDataStream &str, const ComponentItem &item)
{
    str << item.pos();
    str << item.rotation();
    str << item.scale();

    foreach(OutputPin *output, item._outputs) {
        str << output->getValue();
    }

    return str;
}

QDataStream &operator>>(QDataStream &str, ComponentItem &item)
{
    QPointF pos;
    str >> pos;
    item.setPos(pos);

    qreal rotation;
    str >> rotation;
    item.setRotation(rotation);

    qreal scale;
    str >> scale;
    item.setScale(scale);

    foreach(OutputPin *output, item._outputs) {
        quint8 value;
        str >> value;
        output->setValue(value);
    }

    return str;
}
