#include "fulladder.hpp"
#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

FullAdderItem::FullAdderItem(Component *component) : ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 100);

    path.moveTo(0, 20);
    path.lineTo(-20, 20); // A

    path.moveTo(0, 40);
    path.lineTo(-20, 40); // B

    path.moveTo(0, 60);
    path.lineTo(-20, 60); // Cin

    path.moveTo(70, 30);
    path.lineTo(90, 30); // Sum

    path.moveTo(70, 60);
    path.lineTo(90, 60); // Cout

    setPath(path);

    QGraphicsTextItem *aLabel = new QGraphicsTextItem("A", this);
    aLabel->setPos(0.5, 10);
    aLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *bLabel = new QGraphicsTextItem("B", this);
    bLabel->setPos(0.5, 30);
    bLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *cinLabel = new QGraphicsTextItem("Cin", this);
    cinLabel->setPos(0.5, 50);
    cinLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *sumLabel = new QGraphicsTextItem("Sum", this);
    sumLabel->setPos(40, 20);
    sumLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *coutLabel = new QGraphicsTextItem("Cout", this);
    coutLabel->setPos(37, 50);
    coutLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 15, 270);  // A
    _inputs << new InputPin(this, -20, 35, 270);  // B
    _inputs << new InputPin(this, -20, 55, 270);  // Cin

    _outputs << new OutputPin(this, 80, 25, 90);  // Sum
    _outputs << new OutputPin(this, 80, 55, 90);  // Cout

    updateOutputs();
}

void FullAdderItem::updateOutputs()
{
    bool A = _inputs[0]->value();
    bool B = _inputs[1]->value();
    bool Cin = _inputs[2]->value();

    bool Sum = A ^ B ^ Cin;
    bool Cout = (A && B) || (A && Cin) || (B && Cin);

    _outputs[0]->setValue(Sum);
    _outputs[1]->setValue(Cout);
}

FullAdder::FullAdder(QObject* parent) :
    Component(parent)
{
}

QString FullAdder::category() const
{
    return "ARITHMETIC BLOCKS";
}

QString FullAdder::name() const
{
    return "FULL ADDER";
}

ComponentItem *FullAdder::item()
{
    return new FullAdderItem(this);
}
