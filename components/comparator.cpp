#include "comparator.hpp"
#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

ComparatorItem::ComparatorItem(Component *component) : ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 80);

    path.moveTo(0, 20);
    path.lineTo(-20, 20); // A

    path.moveTo(0, 40);
    path.lineTo(-20, 40); // B

    path.moveTo(70, 20);
    path.lineTo(90, 20); // A==B

    path.moveTo(70, 40);
    path.lineTo(90, 40); // A<B

    path.moveTo(70, 60);
    path.lineTo(90, 60); // A>B

    setPath(path);

    QGraphicsTextItem *aLabel = new QGraphicsTextItem("A", this);
    aLabel->setPos(0.5, 10);
    aLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *bLabel = new QGraphicsTextItem("B", this);
    bLabel->setPos(0.5, 30);
    bLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *eqLabel = new QGraphicsTextItem("A==B", this);
    eqLabel->setPos(32, 10);
    eqLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *ltLabel = new QGraphicsTextItem("A<B", this);
    ltLabel->setPos(40, 30);
    ltLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *gtLabel = new QGraphicsTextItem("A>B", this);
    gtLabel->setPos(40, 50);
    gtLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 15, 270);  // A
    _inputs << new InputPin(this, -20, 35, 270);  // B

    _outputs << new OutputPin(this, 80, 15, 90);  // A==B
    _outputs << new OutputPin(this, 80, 35, 90);  // A<B
    _outputs << new OutputPin(this, 80, 55, 90);  // A>B

    updateOutputs();
}

void ComparatorItem::updateOutputs()
{
    bool A = _inputs[0]->value();
    bool B = _inputs[1]->value();

    bool eq = (A == B);
    bool lt = (!A && B);
    bool gt = (A && !B);

    _outputs[0]->setValue(eq);
    _outputs[1]->setValue(lt);
    _outputs[2]->setValue(gt);
}

Comparator::Comparator(QObject* parent) :
    Component(parent)
{
}

QString Comparator::category() const
{
    return "ARITHMETIC BLOCKS";
}

QString Comparator::name() const
{
    return "COMPARATOR";
}

ComponentItem *Comparator::item()
{
    return new ComparatorItem(this);
}
