#pragma once

#include "componentitem.hpp"
#include "pinitem.hpp"
#include "wire.hpp"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QPixmap>
#include <QPen>

class Component : public QObject
{
    Q_OBJECT

public:
    Component(QObject *parent);
    virtual ~Component() = default;

    virtual QString name() const = 0;
    virtual QString category() const = 0;
    QPixmap icon();

    virtual ComponentItem *item() = 0;
};
