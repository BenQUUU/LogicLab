#include "rsflipflop.hpp"

#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

#include <QPen>

RSFlipFlopItem::RSFlipFlopItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 50);

    // (S)
    path.moveTo(0, 15);
    path.lineTo(-20, 15);

    // (R)
    path.moveTo(0, 35);
    path.lineTo(-20, 35);

    // Q
    path.moveTo(70, 15);
    path.lineTo(90, 15);

    // (¬Q)
    path.moveTo(70, 35);
    path.lineTo(90, 35);

    setPath(path);

    QGraphicsTextItem *setLabel = new QGraphicsTextItem("R", this);
    setLabel->setPos(1, 3);
    setLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *resetLabel = new QGraphicsTextItem("S", this);
    resetLabel->setPos(1, 22);
    resetLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *qLabel = new QGraphicsTextItem("Q", this);
    qLabel->setPos(52, 3);
    qLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *notQLabel = new QGraphicsTextItem("~Q", this);
    notQLabel->setPos(45, 22);
    notQLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 10, 270);  // Set (S)
    _inputs << new InputPin(this, -20, 30, 270);  // Reset (R)

    _outputs << new OutputPin(this, 80, 10, 90);  // Q
    _outputs << new OutputPin(this, 80, 30, 90);  // ¬Q

    updateOutputs();
}

void RSFlipFlopItem::updateOutputs()
{
    bool R = _inputs[0]->value();
    bool S = _inputs[1]->value();


    if (!R && S) {
        _Q = true;
        _notQ = false;
    } else if (R && !S) {
        _Q = false;
        _notQ = true;
    } else if (R && S) {
        _Q = false;
        _notQ = true;
    } else {}


    _outputs[0]->setValue(_Q);
    _outputs[1]->setValue(_notQ);
}

RSFlipFlop::RSFlipFlop(QObject *parent) :
    Component(parent)
{ }

QString RSFlipFlop::category() const
{
    return "FLIP FLOPS";
}

QString RSFlipFlop::name() const
{
    return "RS FLIP-FLOP";
}

ComponentItem *RSFlipFlop::item()
{
    return new RSFlipFlopItem(this);
}
