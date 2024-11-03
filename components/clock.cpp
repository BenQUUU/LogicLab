#include "clock.hpp"

#include "../core/inputpin.hpp"
#include "../core/outputpin.hpp"

#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDebug>
#include <QLabel>
#include <QDialogButtonBox>

ClockProperties::ClockProperties(ClockItem* timer, QWidget* parent) :
    QDialog(parent),
    _timer(timer),
    _signalSpinBox(new QSpinBox),
    _repeatSpinBox(new QSpinBox)
{
    _signalSpinBox->setMinimum(1);
    _signalSpinBox->setMaximum(INT_MAX);
    _signalSpinBox->setSuffix(" ms");

    // _repeatSpinBox->setMinimum(1);
    // _repeatSpinBox->setMaximum(INT_MAX);
    // _repeatSpinBox->setSuffix(" ms");

    _signalSpinBox->setValue(_timer->_signalTimer.interval());
    _repeatSpinBox->setValue(_timer->_repeatTimer.interval());

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), SLOT(reject()));

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("Pulse every", _signalSpinBox);
    //layout->addRow("Pulse lasts:", _repeatSpinBox);
    layout->addWidget(buttons);
    setLayout(layout);
}

ClockProperties::~ClockProperties()
{
    delete _signalSpinBox;
    _signalSpinBox = 0;

    delete _repeatSpinBox;
    _repeatSpinBox = 0;
}

void ClockProperties::accept()
{
    _timer->_signalTimer.start(_signalSpinBox->value());
    _timer->_repeatTimer.setInterval(_repeatSpinBox->value());

    QDialog::accept();
}

ClockItem::ClockItem(Component* component) :
    ComponentItem(component)
{
    connect(&_signalTimer, SIGNAL(timeout()), SLOT(signalTimeout()));
    connect(&_repeatTimer, SIGNAL(timeout()), SLOT(repeatTimeout()));

    _signalTimer.setSingleShot(true);
    _repeatTimer.setSingleShot(true);

    _signalTimer.start(1000);
    _repeatTimer.setInterval(1000);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.addRect(0, 0, 50, 50);

    path.moveTo(50, 25);
    path.lineTo(65, 25);

    path.moveTo(5, 40);
    path.lineTo(15, 40);

    path.lineTo(15, 10);

    path.lineTo(35, 10);

    path.lineTo(35, 40);

    path.lineTo(45, 40);

    setPath(path);

    _outputs << new OutputPin(this, 60, 20, 90);
}

bool ClockItem::hasProperties() const
{
    return true;
}

void ClockItem::showProperties()
{
    ClockProperties dlg(this);
    dlg.exec();
}

void ClockItem::signalTimeout()
{
    _outputs[0]->setValue(true);
    _repeatTimer.start();
}

void ClockItem::repeatTimeout()
{
    _outputs[0]->setValue(false);
    _signalTimer.start();
}

Clock::Clock(QObject* parent) :
    Component(parent)
{
}

QString Clock::category() const
{
    return "Inputs";
}

QString Clock::name() const
{
    return "Clock";
}

ComponentItem* Clock::item()
{
    return new ClockItem(this);
}
