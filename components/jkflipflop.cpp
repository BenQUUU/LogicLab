#include "jkflipflop.hpp"
#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

JKFlipFlopItem::JKFlipFlopItem(Component *component) : ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 70);

    // (J)
    path.moveTo(0, 15);
    path.lineTo(-20, 15);

    // (CLK)
    path.moveTo(0, 35);
    path.lineTo(-20, 35);

    // (K)
    path.moveTo(0, 55);
    path.lineTo(-20, 55);

    // Q
    path.moveTo(70, 15);
    path.lineTo(90, 15);

    // Q (¬Q)
    path.moveTo(70, 55);
    path.lineTo(90, 55);

    setPath(path);


    QGraphicsTextItem *jLabel = new QGraphicsTextItem("J", this);
    jLabel->setPos(0.5, 5);
    jLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *clockLabel = new QGraphicsTextItem("CLK", this);
    clockLabel->setPos(0.5, 25);
    clockLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *kLabel = new QGraphicsTextItem("K", this);
    kLabel->setPos(0.5, 45);
    kLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *qLabel = new QGraphicsTextItem("Q", this);
    qLabel->setPos(52, 5);
    qLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *notQLabel = new QGraphicsTextItem("~Q", this);
    notQLabel->setPos(45, 45);
    notQLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 10, 270);  // J (J)
    _inputs << new InputPin(this, -20, 30, 270);  // Clock (CLK)
    _inputs << new InputPin(this, -20, 50, 270);  // K (K)

    _outputs << new OutputPin(this, 80, 10, 90);  // Q
    _outputs << new OutputPin(this, 80, 50, 90);  // ¬Q

    updateOutputs();
}

void JKFlipFlopItem::updateOutputs()
{
    bool J = _inputs[0]->value();
    bool CLK = _inputs[1]->value();
    bool K = _inputs[2]->value();

    if (CLK) {
        if (J && !K) {
            _Q = true;
            _notQ = false;
        } else if (!J && K) {
            _Q = false;
            _notQ = true;
        } else if (J && K) {
            _Q = !_Q;
            _notQ = !_Q;
        }
    }

    _outputs[0]->setValue(_Q);
    _outputs[1]->setValue(_notQ);
}

JKFlipFlop::JKFlipFlop(QObject *parent) : Component(parent) {}

QString JKFlipFlop::category() const
{
    return "FLIP FLOPS";
}

QString JKFlipFlop::name() const
{
    return "JK Flip-Flop";
}

ComponentItem *JKFlipFlop::item()
{
    return new JKFlipFlopItem(this);
}
