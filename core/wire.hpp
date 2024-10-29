#pragma once

#include <QGraphicsPathItem>

class OutputPin;
class InputPin;

class Wire : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    Wire(InputPin* input, OutputPin* output);
    Wire(const QPointF& start, const QPointF& end);
    ~Wire();

    QRectF boundingRect() const;

    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const { return Type; }

    bool getValue() const;

    QPointF startPos() const;
    void setStartPos(const QPointF &start);

    QPointF endPos() const;
    void setEndPos(const QPointF &end);

    void updatePath();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:
    void updateColor(bool value);

private:
    InputPin* _input;
    OutputPin* _output;

    QPointF _startPoint;
    QPointF _endPoint;
};
