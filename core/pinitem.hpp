#pragma once

#include <QGraphicsEllipseItem>
#include <QPointer>
#include <QObject>

class ComponentItem;

class PinItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT;

public:
    static QColor colorForValue(bool value);

    PinItem(ComponentItem* parent, double x, double y, int angle = -1);

    int angle() const { return _angle; }

    virtual void updateWires() = 0;

    virtual ~PinItem() = default;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    int _angle;
};
