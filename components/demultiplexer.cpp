#include "demultiplexer.hpp"
#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

DemultiplexerItem::DemultiplexerItem(Component *component) : ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 100);

    path.moveTo(70, 20);
    path.lineTo(90, 20); // Q0

    path.moveTo(70, 40);
    path.lineTo(90, 40); // Q1

    path.moveTo(70, 60);
    path.lineTo(90, 60); // Q2

    path.moveTo(70, 80);
    path.lineTo(90, 80); // Q3

    path.moveTo(30, 100);
    path.lineTo(30, 120); // S0

    path.moveTo(40, 100);
    path.lineTo(40, 120); // S1

    path.moveTo(35, 0);
    path.lineTo(35, -20); // EN

    path.moveTo(0, 45);
    path.lineTo(-20, 45); // D

    setPath(path);

    const QStringList labels {"Q0", "Q1", "Q2", "Q3"};
    constexpr int positions[] = { 10, 30, 50, 70 }; // Y positions for text items

    for (int i = 0; i < labels.size(); ++i) {
        QGraphicsTextItem *label = new QGraphicsTextItem(labels[i], this);
        label->setPos(46, positions[i]);
        label->setDefaultTextColor(Qt::black);
    }

    QGraphicsTextItem *s0Label = new QGraphicsTextItem("S0", this);
    s0Label->setPos(18, 80);
    s0Label->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *s1Label = new QGraphicsTextItem("S1", this);
    s1Label->setPos(32, 80);
    s1Label->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *enLabel = new QGraphicsTextItem("EN", this);
    enLabel->setPos(25, 0.2);
    enLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *qLabel = new QGraphicsTextItem("D", this);
    qLabel->setPos(0.5, 30);
    qLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 40, 90);  // D

    _inputs << new InputPin(this, 25, 115, 270);  // S0
    _inputs << new InputPin(this, 35, 115, 270);  // S1
    _inputs << new InputPin(this, 30, -20, 270); // EN

    _outputs << new OutputPin(this, 80, 15, 270);
    _outputs << new OutputPin(this, 80, 35, 270);
    _outputs << new OutputPin(this, 80, 55, 270);
    _outputs << new OutputPin(this, 80, 75, 270);

    updateOutputs();
}

void DemultiplexerItem::updateOutputs()
{
    bool D = _inputs[0]->value();
    bool S0 = _inputs[1]->value();
    bool S1 = _inputs[2]->value();
    _enabled = _inputs[3]->value();

    if (_enabled) {
        _out0 = (!S1 && !S0) ? D : false;
        _out1 = (!S1 && S0) ? D : false;
        _out2 = (S1 && !S0) ? D : false;
        _out3 = (S1 && S0) ? D : false;
    } else {
        _out0 = _out1 = _out2 = _out3 = false;
    }

    _outputs[0]->setValue(_out0);
    _outputs[1]->setValue(_out1);
    _outputs[2]->setValue(_out2);
    _outputs[3]->setValue(_out3);
}

Demultiplexer::Demultiplexer(QObject* parent) :
    Component(parent)
{
}

QString Demultiplexer::category() const
{
    return "SELECTORS";
}

QString Demultiplexer::name() const
{
    return "DEMULTIPLEXER";
}

ComponentItem *Demultiplexer::item()
{
    return new DemultiplexerItem(this);
}
