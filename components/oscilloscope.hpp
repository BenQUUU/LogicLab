#pragma once

#include "../core/component.hpp"
#include <QChartView>
#include <QLineSeries>
#include <QTimer>

class OscilloscopeItem : public ComponentItem
{
    Q_OBJECT

public:
    OscilloscopeItem(Component* component);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void updateData();

private:
    QTimer* timer;
    QDialog* chartDialog;
    QChartView* chartView;
    QLineSeries* series;
    int timeCounter;
};

class Oscilloscope : public Component
{
    Q_OBJECT

public:
    Oscilloscope(QObject* parent = 0);

    QString category() const;
    QString name() const;

    ComponentItem* item();
};
