#include "wire.hpp"

#include "component.hpp"
#include "inputpin.hpp"
#include "outputpin.hpp"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <qmath.h>

Wire::Wire(InputPin* input, OutputPin* output) :
    _input(input),
    _output(output),
    _startPoint(QPointF(-1.0, -1.0)),
    _endPoint(QPointF(-1.0, -1.0))
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

    updateColor(_output->getValue());
    updatePath();

    _input->setWire(this);
    _output->addWire(this);

    connect(_output, SIGNAL(valueChanged(bool)), SLOT(updateColor(bool)));
    connect(_output, SIGNAL(valueChanged(bool)), _input, SIGNAL(valueChanged(bool)));
}

Wire::Wire(const QPointF &start, const QPointF &end) :
    _input(0),
    _output(0),
    _startPoint(start),
    _endPoint(end)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

    updateColor(false);
    updatePath();
}

Wire::~Wire()
{
    if(!_input && !_output)
    {
        return;
    }

    _input->setWire(0);
    _output->removeWire(this);

    disconnect(_output, SIGNAL(valueChanged(bool)), this, SLOT(updateColor(bool)));
    disconnect(_output, SIGNAL(valueChanged(bool)), _input, SIGNAL(valueChanged(bool)));
}

QRectF Wire::boundingRect() const
{
    QRectF rect = QGraphicsPathItem::boundingRect();

    return rect.adjusted(-2.0, -2.0, 2.0, 2.0);
}

bool Wire::getValue() const
{
    if(!_output)
    {
        return false;
    }

    return _output->getValue();
}

QPointF Wire::startPos() const
{
    if(_startPoint != QPointF(-1.0, -1.0))
    {
        return _startPoint;
    }

    if(_input)
    {
        return _input->scenePos() + _input->boundingRect().center();
    }

    return QPointF(-1.0, -1.0);
}

void Wire::setStartPos(const QPointF &start)
{
    _startPoint = start;
    updatePath();
}

QPointF Wire::endPos() const
{
    if(_endPoint != QPointF(-1.0, -1.0))
    {
        return _endPoint;
    }

    if(_output)
    {
        return _output->scenePos() + _output->boundingRect().center();
    }

    return QPointF(-1.0, -1.0);
}

void Wire::setEndPos(const QPointF &end)
{
    _endPoint = end;
    updatePath();
}

float distance(const QPointF &p1, const QPointF &p2)
{
    float vx, vy;

    vx = p2.x() - p1.x();
    vy = p2.y() - p1.y();

    return qSqrt( vx * vx + vy * vy );
}

void Wire::updatePath()
{
    prepareGeometryChange();

    QPointF start = startPos();
    QPointF end = endPos();

    // Upewniamy się, że przewód zaczyna się na niższym poziomie Y
    if (start.y() > end.y()) {
        qSwap(start, end);
    }

    QPainterPath path;
    path.moveTo(start);

    // Dodajemy punkty pośrednie dla prostopadłego prowadzenia przewodu
    // Punkty będą zależne od tego, czy przewód idzie bardziej w pionie czy w poziomie
    if (qAbs(start.x() - end.x()) > qAbs(start.y() - end.y())) {
        // Większy dystans w poziomie, więc robimy zakręt w pionie po środku
        QPointF middlePoint((start.x() + end.x()) / 2, start.y());
        path.lineTo(middlePoint);
        middlePoint.setY(end.y());
        path.lineTo(middlePoint);
    } else {
        // Większy dystans w pionie, więc robimy zakręt w poziomie po środku
        QPointF middlePoint(start.x(), (start.y() + end.y()) / 2);
        path.lineTo(middlePoint);
        middlePoint.setX(end.x());
        path.lineTo(middlePoint);
    }

    path.lineTo(end); // końcowy odcinek do końca przewodu

    setPath(path);
}

QVariant Wire::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change ==  QGraphicsItem::ItemSelectedChange) {
        if(value.toBool()) {
            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
            effect->setOffset(0, 0);
            effect->setColor(QColor(130, 180, 10));
            effect->setBlurRadius(8 * scene()->views().first()->transform().m11());
            setGraphicsEffect(effect);
        } else {
            setGraphicsEffect(0);
        }
    }

    return QGraphicsPathItem::itemChange(change, value);
}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap));
    painter->drawPath(path());

    painter->setPen(pen());
    painter->drawPath(path());
}

void Wire::updateColor(bool value)
{
    setPen(QPen(PinItem::colorForValue(value), 4.0, Qt::SolidLine, Qt::RoundCap));
    update();
}
