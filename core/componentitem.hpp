#pragma once

#include <QObject>
#include <QGraphicsPathItem>

class Component;
class OutputPin;
class InputPin;

class ComponentItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)

public:
    ComponentItem(Component* component);

    const Component* component() const { return _component; }

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    virtual bool hasProperties() const { return false; }

    virtual void showProperties() {}

    virtual ~ComponentItem() = default;

public slots:
    void update();

    virtual void updateOutputs() {}

protected:
    Component* _component;
    QVector<InputPin*> _inputs;
    QVector<OutputPin*> _outputs;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    friend QDataStream& operator<<(QDataStream& str, const ComponentItem& item);
    friend QDataStream& operator>>(QDataStream& str, ComponentItem& item);
};
