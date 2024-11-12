#include "multiplexer.hpp"
#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

MultiplexerItem::MultiplexerItem(Component *component) : ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 100);

    path.moveTo(0, 20);
    path.lineTo(-20, 20); // D0

    path.moveTo(0, 40);
    path.lineTo(-20, 40); // D1

    path.moveTo(0, 60);
    path.lineTo(-20, 60); // D2

    path.moveTo(0, 80);
    path.lineTo(-20, 80); // D3

    path.moveTo(30, 100);
    path.lineTo(30, 120); // S0

    path.moveTo(40, 100);
    path.lineTo(40, 120); // S1

    path.moveTo(35, 0);
    path.lineTo(35, -20); // EN

    path.moveTo(70, 45);
    path.lineTo(90, 45); // Q

    setPath(path);

    const QStringList labels {"D0", "D1", "D2", "D3"};
    constexpr int positions[] = { 10, 30, 50, 70 }; // Y positions for text items
    for (int i = 0; i < labels.size(); ++i) {
        QGraphicsTextItem *label = new QGraphicsTextItem(labels[i], this);
        label->setPos(0.5, positions[i]);
        label->setDefaultTextColor(Qt::black);
    }

    QGraphicsTextItem *s0Label = new QGraphicsTextItem("S0", this);
    s0Label->setPos(18, 80);
    s0Label->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *s1Label = new QGraphicsTextItem("S1", this);
    s1Label->setPos(32, 80);
    s1Label->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *enLabel = new QGraphicsTextItem("EN", this);
    enLabel->setPos(25, 0.5);
    enLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *qLabel = new QGraphicsTextItem("Q", this);
    qLabel->setPos(52, 30);
    qLabel->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 15, 270);  // D0-D3
    _inputs << new InputPin(this, -20, 35, 270);  // D0-D3
    _inputs << new InputPin(this, -20, 55, 270);  // D0-D3
    _inputs << new InputPin(this, -20, 75, 270);  // D0-D3

    _inputs << new InputPin(this, 25, 115, 270);  // S0
    _inputs << new InputPin(this, 35, 115, 270);  // S1
    _inputs << new InputPin(this, 30, -20, 270); // EN

    _outputs << new OutputPin(this, 80, 40, 90);  // Q

    updateOutputs();
}

void MultiplexerItem::updateOutputs()
{
    bool D0 = _inputs[0]->value();
    bool D1 = _inputs[1]->value();
    bool D2 = _inputs[2]->value();
    bool D3 = _inputs[3]->value();
    bool S0 = _inputs[4]->value();
    bool S1 = _inputs[5]->value();
    _enabled = _inputs[6]->value();

    if (_enabled) {
        if (!S1 && !S0) {
            _output = D0;
        } else if (!S1 && S0) {
            _output = D1;
        } else if (S1 && !S0) {
            _output = D2;
        } else if (S1 && S0) {
            _output = D3;
        }
    } else {
        _output = false;
    }

    _outputs[0]->setValue(_output);
}

Multiplexer::Multiplexer(QObject* parent) :
    Component(parent)
{
}

QString Multiplexer::category() const
{
    return "SELECTORS";
}

QString Multiplexer::name() const
{
    return "Multiplexer";
}

ComponentItem *Multiplexer::item()
{
    return new MultiplexerItem(this);
}
