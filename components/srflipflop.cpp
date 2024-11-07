#include "srflipflop.hpp"

#include "../core/outputpin.hpp"
#include "../core/inputpin.hpp"

#include <QPen>

SRFlipFlopItem::SRFlipFlopItem(Component *component) :
    ComponentItem(component)
{
    QPainterPath path;
    path.addRect(0, 0, 70, 50);  // Tworzenie prostokąta o wymiarach 70x50

    // Linia do wejścia Set (S)
    path.moveTo(0, 15);
    path.lineTo(-20, 15);

    // Linia do wejścia Reset (R)
    path.moveTo(0, 35);
    path.lineTo(-20, 35);

    // Linia do wyjścia Q
    path.moveTo(70, 15);
    path.lineTo(90, 15);

    // Linia do wyjścia negacji Q (¬Q)
    path.moveTo(70, 35);
    path.lineTo(90, 35);

    setPath(path);

    // Dodanie tekstu "SR" na środku prostokąta
    QGraphicsTextItem *srText = new QGraphicsTextItem("SR", this);
    srText->setPos(25, 15); // Pozycja napisu "SR" w centrum prostokąta (dostosuj w zależności od rozmiaru prostokąta)
    srText->setDefaultTextColor(Qt::black); // Ustawienie koloru tekstu na czarny

    // Dodanie podpisów do pinów wejściowych i wyjściowych
    QGraphicsTextItem *setLabel = new QGraphicsTextItem("S", this);
    setLabel->setPos(1, 3); // Pozycja napisu "Set" przy wejściu S
    setLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *resetLabel = new QGraphicsTextItem("R", this);
    resetLabel->setPos(1, 22); // Pozycja napisu "Reset" przy wejściu R
    resetLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *qLabel = new QGraphicsTextItem("Q", this);
    qLabel->setPos(52, 3); // Pozycja napisu "Q" przy wyjściu Q
    qLabel->setDefaultTextColor(Qt::black);

    QGraphicsTextItem *notQLabel = new QGraphicsTextItem("~Q", this);
    notQLabel->setPos(45, 22); // Pozycja napisu "¬Q" przy wyjściu ¬Q
    notQLabel->setDefaultTextColor(Qt::black);

    // Dodanie pinów
    _inputs << new InputPin(this, -20, 10, 270);  // Set (S)
    _inputs << new InputPin(this, -20, 30, 270);  // Reset (R)

    // Dodanie wyjść Q i ¬Q
    _outputs << new OutputPin(this, 80, 10, 90);  // Q
    _outputs << new OutputPin(this, 80, 30, 90);  // ¬Q

    updateOutputs();
}

void SRFlipFlopItem::updateOutputs()
{
    bool S = _inputs[0]->value();  // Odczyt wartości wejścia Set (S)
    bool R = _inputs[1]->value();  // Odczyt wartości wejścia Reset (R)

    // Logika przerzutnika SR:
    if (S == true && R == false) {
        // Ustawienie Q na 1 (logiczna "1") i ~Q na 0
        _Q = true;
        _notQ = false;
    } else if (S == false && R == true) {
        // Reset Q na 0 (logiczna "0") i ~Q na 1
        _Q = false;
        _notQ = true;
    } else if (S == true && R == true) {
        // Stan nieokreślony: Możemy ustawić Q na 0 i ~Q na 1
        _Q = false;
        _notQ = true;  // Można dodać komunikat błędu lub stan nieokreślony
    } else {
        // S = 0, R = 0: Zachowanie poprzedniego stanu (pamięć)
        // Nie zmieniamy wartości Q i ~Q, tylko pamiętamy poprzednią wartość
    }

    // Aktualizacja wartości wyjść
    _outputs[0]->setValue(_Q);      // Aktualizacja wyjścia Q (0 lub 255)
    _outputs[1]->setValue(_notQ);   // Aktualizacja wyjścia ¬Q (odwrotność Q)
}

SRFlipFlop::SRFlipFlop(QObject *parent) :
    Component(parent)
{
}

QString SRFlipFlop::category() const
{
    return "FLIP FLOPS";
}

QString SRFlipFlop::name() const
{
    return "SR FLIP FLOP";
}

ComponentItem *SRFlipFlop::item()
{
    return new SRFlipFlopItem(this);
}
