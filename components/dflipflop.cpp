#include "dflipflop.hpp"
#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

DFlipFlopItem::DFlipFlopItem(Component *component) : ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 50);

    // (D)
    path.moveTo(0, 15);
    path.lineTo(-20, 15);

    // (CLK)
    path.moveTo(0, 35);
    path.lineTo(-20, 35);

    // Q
    path.moveTo(70, 15);
    path.lineTo(90, 15);

    // (¬Q)
    path.moveTo(70, 35);
    path.lineTo(90, 35);

    setPath(path);

    QGraphicsTextItem *dataLabel = new QGraphicsTextItem("D", this);
    dataLabel->setPos(1, 3);
    dataLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *clockLabel = new QGraphicsTextItem("CLK", this);
    clockLabel->setPos(1, 22);
    clockLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *qLabel = new QGraphicsTextItem("Q", this);
    qLabel->setPos(52, 3);
    qLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *notQLabel = new QGraphicsTextItem("~Q", this);
    notQLabel->setPos(45, 22);
    notQLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 10, 270);  // Data (D)
    _inputs << new InputPin(this, -20, 30, 270);  // Clock (CLK)

    _outputs << new OutputPin(this, 80, 10, 90);  // Q
    _outputs << new OutputPin(this, 80, 30, 90);  // ¬Q

    updateOutputs();
}

void DFlipFlopItem::updateOutputs()
{
    bool D = _inputs[0]->value();
    bool CLK = _inputs[1]->value();

    if (CLK) {
        _Q = D;
        _notQ = !D;
    }

    _outputs[0]->setValue(_Q);
    _outputs[1]->setValue(_notQ);
}

DFlipFlop::DFlipFlop(QObject *parent) : Component(parent) {}

QString DFlipFlop::category() const
{
    return "FLIP FLOPS";
}

QString DFlipFlop::name() const
{
    return "D Flip-Flop";
}

ComponentItem *DFlipFlop::item()
{
    return new DFlipFlopItem(this);
}
