#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "scene.hpp"
#include "componenttree.hpp"
#include "core/componentitem.hpp"
#include "core/component.hpp"

#include "components/andgate.hpp"
#include "components/orgate.hpp"
#include "components/button.hpp"
#include "components/xorgate.hpp"
#include "components/clock.hpp"
#include "components/nandgate.hpp"
#include "components/notgate.hpp"
#include "components/norgate.hpp"
#include "components/xnorgate.hpp"
#include "components/switch.hpp"
#include "components/led.hpp"
#include "components/srflipflop.hpp"
#include "components/rsflipflop.hpp"
#include "components/dflipflop.hpp"
#include "components/tflipflop.hpp"
#include "components/jkflipflop.hpp"
#include "components/digit.hpp"
#include "components/oscilloscope.hpp"
#include "components/multiplexer.hpp"
#include "components/demultiplexer.hpp"
#include "components/fulladder.hpp"
#include "components/comparator.hpp"
#include "components/logicallow.hpp"
#include "components/logicalhigh.hpp"

#include <QDragEnterEvent>
#include <QGraphicsItem>
#include <QPluginLoader>
#include <QSignalMapper>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolButton>
#include <QSettings>
#include <QToolTip>
#include <qmath.h>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _scene(new Scene),
    _zoomSlider(new QSlider),
    _mruMapper(new QSignalMapper)
{
    _ui->setupUi(this);

    _scene->setSceneRect(QRect(QPoint(), QSize(2000, 2000)));
    _ui->graphicsView->setScene(_scene);
    _ui->graphicsView->installEventFilter(this);

    createZoom();
    connectActions();
    updateActions();

    setCurrentFile();
    loadComponents();
}

MainWindow::~MainWindow()
{
    delete _scene;
    _scene = nullptr;

    delete _mruMapper;
    _mruMapper = nullptr;

    delete _ui;
    _ui = nullptr;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
        return;
    }

    event->ignore();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == _ui->graphicsView) {
        if (e->type() == QEvent::ContextMenu) {
            const QContextMenuEvent *event = dynamic_cast<QContextMenuEvent*>(e);

            if (_scene->selectedComponents().count()) {
                QMenu menu;
                menu.addAction(_ui->cutAction);
                menu.addAction(_ui->copyAction);
                menu.addAction(_ui->pasteAction);
                menu.addAction(_ui->deleteAction);
                menu.addSeparator();
                menu.addMenu(_ui->rotateMenu);

                if ((_scene->selectedComponents().count() == 1) && _scene->selectedComponent()->hasProperties()) {
                    menu.addSeparator();
                    menu.addAction(_ui->propertiesAction);
                }

                menu.exec(_ui->graphicsView->mapToGlobal(event->pos()));
            }
        }
    }

    return QMainWindow::eventFilter(obj, e);
}

void MainWindow::newFile()
{
    if (!maybeSave()) return;

    _scene->clear();
    _scene->setModified(false);
    _zoomSlider->setValue(0);
    setCurrentFile();
}

void MainWindow::open()
{
    if (!maybeSave()) return;

    const QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty()) loadFile(fileName);
}

bool MainWindow::save()
{
    if (_fileName.isEmpty()) return saveAs();

    return saveFile(_fileName);
}

bool MainWindow::saveAs()
{
    const QString fileName = QFileDialog::getSaveFileName(this);

    if (fileName.isEmpty()) return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
     QMessageBox::about(0, tr("About LogicLab"), "<b>LogicLab v" + QString::fromUtf8(APP_VERSION));
}

void MainWindow::zoomIn()
{
    _zoomSlider->setValue(_zoomSlider->value() + 1);
}

void MainWindow::zoomOut()
{
    _zoomSlider->setValue(_zoomSlider->value() - 1);
}

void MainWindow::setZoom(const int &value)
{
    const float scale = qPow(1.15, value);

    _ui->graphicsView->setTransform(QTransform::fromScale(scale, scale));

    _ui->zoomInAction->setEnabled(_zoomSlider->value() != _zoomSlider->maximum());
    _ui->zoomOutAction->setEnabled(_zoomSlider->value() != _zoomSlider->minimum());

}

void MainWindow::updateActions()
{
    bool hasComponents = (_scene->selectedComponents().count() > 0);
    bool singleComponent = (_scene->selectedComponents().count() == 1);
    bool hasProperties = (singleComponent && _scene->selectedComponent()->hasProperties());
    bool hasWires = (_scene->selectedWires().count() > 0);

    _ui->copyAction->setEnabled(hasComponents);
    _ui->pasteAction->setEnabled(hasComponents);
    _ui->cutAction->setEnabled(hasComponents);
    _ui->rotateClockwiseAction->setEnabled(hasComponents);
    _ui->rotateAnticlockwiseAction->setEnabled(hasComponents);

    _ui->deleteAction->setEnabled(hasComponents || hasWires);

    _ui->propertiesAction->setEnabled(hasProperties);

    _ui->undoAction->setEnabled(_scene->canUndo());
    _ui->redoAction->setEnabled(_scene->canRedo());
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("LogicLab"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    _scene->clear();

    QDataStream str(&file);
    while (!str.atEnd()) {
        QString className;
        str >> className;

        Component *component = _ui->treeWidget->component(className);
        if (component) {
            ComponentItem *item = component->item();
            str >> *item;

            item->setTransformOriginPoint(item->boundingRect().center());
            _scene->addItem(item);
        }
    }

    setCurrentFile(fileName);
    _scene->setModified(false);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("LogicLab"), tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QDataStream str(&file);

    // TODO: Fix saving
    foreach (QGraphicsItem *item, _scene->items()) {
        if (ComponentItem *componentItem = dynamic_cast<ComponentItem*>(item)) {
            str << QString(componentItem->component()->metaObject()->className());
            str << *componentItem;
        }
    }

    setCurrentFile(fileName);
    _scene->setModified(false);
    statusBar()->showMessage(tr("File saved"), 2000);

    return true;
}

bool MainWindow::maybeSave()
{
    if (_scene->isModified()) {
        const QMessageBox::StandardButton button = QMessageBox::warning(this, tr("LogicLab"), tr("This project has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (button == QMessageBox::Save) {
            return save();
        } else if (button == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    setWindowModified(false);

    const QString shownName = (fileName.isEmpty()) ? "untitled" : fileName;
    setWindowTitle(shownName + "[*]");

    _fileName = fileName;
}

void MainWindow::createZoom()
{
    _zoomSlider->setOrientation(Qt::Horizontal);
    _zoomSlider->setMinimum(-5);
    _zoomSlider->setMaximum(5);
    _zoomSlider->setMaximumWidth(100);
    connect(_zoomSlider, SIGNAL(valueChanged(int)), SLOT(setZoom(int)));

    QToolButton *zoomInButton = new QToolButton();
    zoomInButton->setAutoRaise(true);
    zoomInButton->setDefaultAction(_ui->zoomInAction);

    QToolButton *zoomOutButton = new QToolButton();
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setDefaultAction(_ui->zoomOutAction);
}

void MainWindow::connectActions()
{
    connect(_scene, SIGNAL(selectionChanged()), SLOT(updateActions()));
    connect(_scene, SIGNAL(modificationChanged(bool)), SLOT(setWindowModified(bool)));
    connect(_scene, SIGNAL(canRedoChanged(bool)), SLOT(updateActions()));
    connect(_scene, SIGNAL(canUndoChanged(bool)), SLOT(updateActions()));

    connect(_ui->deleteAction, SIGNAL(triggered()), _scene, SLOT(deleteSelection()));
    connect(_ui->propertiesAction, SIGNAL(triggered()), _scene, SLOT(showProperties()));

    connect(_ui->undoAction, SIGNAL(triggered()), _scene, SLOT(undo()));
    connect(_ui->redoAction, SIGNAL(triggered()), _scene, SLOT(redo()));

    //rotation
    connect(_ui->rotateClockwiseAction, SIGNAL(triggered()), _scene, SLOT(rotateClockwise()));
    connect(_ui->rotateAnticlockwiseAction, SIGNAL(triggered()), _scene, SLOT(rotateAnticlockwise()));
}

void MainWindow::loadComponents()
{
    Component *andGate = new AndGate(this);
    _ui->treeWidget->addComponent(andGate);

    Component *nandGate = new NandGate(this);
    _ui->treeWidget->addComponent(nandGate);

    Component *orGate = new OrGate(this);
    _ui->treeWidget->addComponent(orGate);

    Component *norGate = new NorGate(this);
    _ui->treeWidget->addComponent(norGate);

    Component *xorGate = new XorGate(this);
    _ui->treeWidget->addComponent(xorGate);

    Component *xnorGate = new XnorGate(this);
    _ui->treeWidget->addComponent(xnorGate);

    Component *notGate = new NotGate(this);
    _ui->treeWidget->addComponent(notGate);

    Component *srFlipFlop = new SRFlipFlop(this);
    _ui->treeWidget->addComponent(srFlipFlop);

    Component *rsFlipFlop = new RSFlipFlop(this);
    _ui->treeWidget->addComponent(rsFlipFlop);

    Component *dFlipFlop = new DFlipFlop(this);
    _ui->treeWidget->addComponent(dFlipFlop);

    Component *tFlipFlop = new TFlipFlop(this);
    _ui->treeWidget->addComponent(tFlipFlop);

    Component *jkFlipFlop = new JKFlipFlop(this);
    _ui->treeWidget->addComponent(jkFlipFlop);

    Component *multiplexer = new Multiplexer(this);
    _ui->treeWidget->addComponent(multiplexer);

    Component *demultiplexer = new Demultiplexer(this);
    _ui->treeWidget->addComponent(demultiplexer);

    Component *fulladder = new FullAdder(this);
    _ui->treeWidget->addComponent(fulladder);

    Component *comparator = new Comparator(this);
    _ui->treeWidget->addComponent(comparator);

    Component *button = new Button(this);
    _ui->treeWidget->addComponent(button);

    Component *logicalLow = new LogicalLow(this);
    _ui->treeWidget->addComponent(logicalLow);

    Component *logicalHigh = new LogicalHigh(this);
    _ui->treeWidget->addComponent(logicalHigh);

    Component *clock = new Clock(this);
    _ui->treeWidget->addComponent(clock);

    Component *switchComponent = new Switch(this);
    _ui->treeWidget->addComponent(switchComponent);

    Component *led = new LED(this);
    _ui->treeWidget->addComponent(led);

    Component *digit = new Digit(this);
    _ui->treeWidget->addComponent(digit);

    Component *oscilloscope = new Oscilloscope(this);
    _ui->treeWidget->addComponent(oscilloscope);
}


