#include "oscilloscope.hpp"
#include "../core/inputpin.hpp"
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>

OscilloscopeItem::OscilloscopeItem(Component* component) :
    ComponentItem(component),
    timeCounter(0)
{
    QPainterPath path;
    path.addRect(0, 0, 50, 50);
    path.moveTo(0, 25);
    path.lineTo(-20, 25);
    setPath(path);

    QGraphicsTextItem *Label = new QGraphicsTextItem("OSC", this);
    Label->setPos(10, 15);
    Label->setDefaultTextColor(Qt::black);

    _inputs << new InputPin(this, -20, 20, 90);

    series = new QLineSeries();
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 100);
    chart->axisY()->setRange(0, 1);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartDialog = new QDialog();
    chartDialog->setWindowTitle("Oscilloscope");
    QVBoxLayout* layout = new QVBoxLayout(chartDialog);
    layout->addWidget(chartView);
    chartDialog->setLayout(layout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OscilloscopeItem::updateData);
}

void OscilloscopeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    chartDialog->show();
    timer->start(100);
}

void OscilloscopeItem::updateData()
{
    if (series->count() > 100) {
        series->remove(0);
    }
    series->append(timeCounter, _inputs[0]->value() ? 1 : 0);
    timeCounter++;
    chartView->chart()->axisX()->setRange(timeCounter - 100, timeCounter);
}

Oscilloscope::Oscilloscope(QObject* parent) :
    Component(parent)
{
}

QString Oscilloscope::category() const
{
    return "OUTPUTS";
}

QString Oscilloscope::name() const
{
    return "Oscilloscope";
}

ComponentItem* Oscilloscope::item()
{
    return new OscilloscopeItem(this);
}
