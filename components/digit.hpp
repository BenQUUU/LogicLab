#pragma once

#include "../core/component.hpp"

#include <QTimer>
#include <array>

static constexpr std::array<std::array<char, 8>, 16> segmentChars =
{{
    {{ 1, 1, 1, 1, 1, 1, 0 }}, // 0
    {{ 0, 1, 1, 0, 0, 0, 0 }}, // 1
    {{ 1, 1, 0, 1, 1, 0, 1 }}, // 2
    {{ 1, 1, 1, 1, 0, 0, 1 }}, // 3
    {{ 0, 1, 1, 0, 0, 1, 1 }}, // 4
    {{ 1, 0, 1, 1, 0, 1, 1 }}, // 5
    {{ 1, 0, 1, 1, 1, 1, 1 }}, // 6
    {{ 1, 1, 1, 0, 0, 0, 0 }}, // 7
    {{ 1, 1, 1, 1, 1, 1, 1 }}, // 8
    {{ 1, 1, 1, 1, 0, 1, 1 }}, // 9
    {{ 1, 1, 1, 0, 1, 1, 1 }}, // A
    {{ 0, 0, 1, 1, 1, 1, 1 }}, // B
    {{ 1, 0, 0, 1, 1, 1, 0 }}, // C
    {{ 0, 1, 1, 1, 1, 0, 1 }}, // D
    {{ 1, 0, 0, 1, 1, 1, 1 }}, // E
    {{ 1, 0, 0, 0, 1, 1, 1 }}  // F
}};

class DigitItem : public ComponentItem
{
    Q_OBJECT

public:
    DigitItem(Component* component);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

class Digit : public Component
{
    Q_OBJECT

public:
    Digit(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
