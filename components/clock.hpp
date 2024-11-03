#pragma once

#include "../core/component.hpp"
#include <QGraphicsPathItem>
#include <QDialog>
#include <QTimer>
#include <QPen>

class ClockItem;
class QComboBox;
class QSpinBox;

class ClockProperties : public QDialog
{
    Q_OBJECT

public:
    ClockProperties(ClockItem* clock, QWidget* parent = 0);
    ~ClockProperties();

public slots:
    void accept();

private:
    ClockItem* _timer;
    QSpinBox* _signalSpinBox;
    QSpinBox* _repeatSpinBox;
};

class ClockItem : public ComponentItem
{
    Q_OBJECT

public:
    ClockItem(Component* component);

    bool hasProperties() const;
    void showProperties();

private slots:
    void signalTimeout();
    void repeatTimeout();

private:
    QTimer _signalTimer;
    QTimer _repeatTimer;

    friend class ClockProperties;
};

class Clock : public Component
{
    Q_OBJECT

public:
    Clock(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem *item();
};
