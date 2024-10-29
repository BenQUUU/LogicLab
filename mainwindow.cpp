#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "scene.hpp"
#include "componenttree.hpp"
#include "core/componentitem.hpp"
#include "core/component.hpp"

#include "components/andgate.hpp"
#include "components/orgate.hpp"
#include "components/button.hpp"

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
    m_ui(new Ui::MainWindow),
    m_scene(new Scene),
    m_zoomSlider(new QSlider),
    m_mruMapper(new QSignalMapper)
{
    m_ui->setupUi(this);

    m_scene->setSceneRect(QRect(QPoint(), QSize(2000, 2000)));
    m_ui->graphicsView->setScene(m_scene);
    m_ui->graphicsView->installEventFilter(this);

    createZoom();
    connectActions();
    updateActions();

    setCurrentFile();
    loadPlugins();
}

MainWindow::~MainWindow()
{
    delete m_scene;
    m_scene = 0;

    delete m_mruMapper;
    m_mruMapper = 0;

    delete m_ui;
    m_ui = 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()) {
        event->accept();
        return;
    }

    event->ignore();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == m_ui->graphicsView) {
        if(e->type() == QEvent::ContextMenu) {
            const QContextMenuEvent *event = dynamic_cast<QContextMenuEvent*>(e);

            if(m_scene->selectedComponents().count()) {
                QMenu menu;
                menu.addAction(m_ui->cutAction);
                menu.addAction(m_ui->copyAction);
                menu.addAction(m_ui->pasteAction);
                menu.addAction(m_ui->deleteAction);
                menu.addSeparator();
                menu.addMenu(m_ui->rotateMenu);

                if((m_scene->selectedComponents().count() == 1) && m_scene->selectedComponent()->hasProperties()) {
                    menu.addSeparator();
                    menu.addAction(m_ui->propertiesAction);
                }

                menu.exec(m_ui->graphicsView->mapToGlobal(event->pos()));
            }
        }
    }

    return QMainWindow::eventFilter(obj, e);
}

void MainWindow::newFile()
{
    if(!maybeSave())
        return;

    m_scene->clear();
    m_scene->setModified(false);
    m_zoomSlider->setValue(0);
    setCurrentFile();
}

void MainWindow::open()
{
    if(!maybeSave())
        return;

    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
        loadFile(fileName);
}

bool MainWindow::save()
{
    if(m_fileName.isEmpty()) {
        //return saveAs();
    }

    return saveFile(m_fileName);
}

// bool MainWindow::saveAs()
// {
//     const QString fileName = QFileDialog::getSaveFileName(this);
//     if(fileName.isEmpty())
//         return false;

//     return saveFile(fileName);
// }

void MainWindow::about()
{
     QMessageBox::about(0, tr("About LogicLab"), "<b>LogicFlow v" + QString::fromUtf8(APP_VERSION) + "</b><br>Copyright &copy; Michał Kolczak");
}

void MainWindow::zoomIn()
{
    m_zoomSlider->setValue(m_zoomSlider->value() + 1);
}

void MainWindow::zoomOut()
{
    m_zoomSlider->setValue(m_zoomSlider->value() - 1);
}

void MainWindow::setZoom(const int &value)
{
    const float scale = qPow(1.15, value);

    m_ui->graphicsView->setTransform(QTransform::fromScale(scale, scale));

    m_ui->zoomInAction->setEnabled(m_zoomSlider->value() != m_zoomSlider->maximum());
    m_ui->zoomOutAction->setEnabled(m_zoomSlider->value() != m_zoomSlider->minimum());

}

void MainWindow::updateActions()
{
    bool hasComponents = (m_scene->selectedComponents().count() > 0);
    bool singleComponent = (m_scene->selectedComponents().count() == 1);
    bool hasProperties = (singleComponent && m_scene->selectedComponent()->hasProperties());
    bool hasWires = (m_scene->selectedWires().count() > 0);

    m_ui->copyAction->setEnabled(hasComponents);
    m_ui->pasteAction->setEnabled(hasComponents);
    m_ui->cutAction->setEnabled(hasComponents);
    m_ui->rotateClockwiseAction->setEnabled(hasComponents);
    m_ui->rotateAnticlockwiseAction->setEnabled(hasComponents);

    m_ui->deleteAction->setEnabled(hasComponents || hasWires);

    m_ui->propertiesAction->setEnabled(hasProperties);

    m_ui->undoAction->setEnabled(m_scene->canUndo());
    m_ui->redoAction->setEnabled(m_scene->canRedo());
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Framework"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    m_scene->clear();

    QDataStream str(&file);
    while(!str.atEnd()) {
        QString className;
        str >> className;

        Component *component = m_ui->treeWidget->component(className);
        if(component) {
            ComponentItem *item = component->item();
            str >> *item;

            item->setTransformOriginPoint(item->boundingRect().center());
            m_scene->addItem(item);
        }
    }

    setCurrentFile(fileName);
    m_scene->setModified(false);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Framework"), tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QDataStream str(&file);
    foreach(QGraphicsItem *item, m_scene->items()) {
        if(ComponentItem *componentItem = dynamic_cast<ComponentItem*>(item)) {
            str << QString(componentItem->component()->metaObject()->className());
            str << *componentItem;
        }
    }

    setCurrentFile(fileName);
    m_scene->setModified(false);
    statusBar()->showMessage(tr("File saved"), 2000);

    return true;
}

bool MainWindow::maybeSave()
{
    if(m_scene->isModified()) {
        const QMessageBox::StandardButton button = QMessageBox::warning(this, tr("Framework"), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(button == QMessageBox::Save) {
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

    const QString shownName = (fileName.isEmpty()) ? "untitled.txt" : fileName;
    setWindowTitle(shownName + "[*] - Framework");

    m_fileName = fileName;
}

void MainWindow::createZoom()
{
    m_zoomSlider->setOrientation(Qt::Horizontal);
    m_zoomSlider->setMinimum(-5);
    m_zoomSlider->setMaximum(5);
    m_zoomSlider->setMaximumWidth(100);
    connect(m_zoomSlider, SIGNAL(valueChanged(int)), SLOT(setZoom(int)));

    QToolButton *zoomInButton = new QToolButton();
    zoomInButton->setAutoRaise(true);
    zoomInButton->setDefaultAction(m_ui->zoomInAction);

    QToolButton *zoomOutButton = new QToolButton();
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setDefaultAction(m_ui->zoomOutAction);

    m_ui->statusBar->addPermanentWidget(zoomOutButton);
    m_ui->statusBar->addPermanentWidget(m_zoomSlider);
    m_ui->statusBar->addPermanentWidget(zoomInButton);
}

void MainWindow::connectActions()
{
    connect(m_scene, SIGNAL(selectionChanged()), SLOT(updateActions()));
    connect(m_scene, SIGNAL(modificationChanged(bool)), SLOT(setWindowModified(bool)));
    connect(m_scene, SIGNAL(canRedoChanged(bool)), SLOT(updateActions()));
    connect(m_scene, SIGNAL(canUndoChanged(bool)), SLOT(updateActions()));
    connect(m_mruMapper, SIGNAL(mapped(QString)), SLOT(loadMruFile(QString)));

    connect(m_ui->fileMenu, SIGNAL(aboutToShow()), SLOT(updateMruMenu()));
    connect(m_ui->deleteAction, SIGNAL(triggered()), m_scene, SLOT(deleteSelection()));
    connect(m_ui->propertiesAction, SIGNAL(triggered()), m_scene, SLOT(showProperties()));

    connect(m_ui->undoAction, SIGNAL(triggered()), m_scene, SLOT(undo()));
    connect(m_ui->redoAction, SIGNAL(triggered()), m_scene, SLOT(redo()));

    //rotation
    connect(m_ui->rotateClockwiseAction, SIGNAL(triggered()), m_scene, SLOT(rotateClockwise()));
    connect(m_ui->rotateAnticlockwiseAction, SIGNAL(triggered()), m_scene, SLOT(rotateAnticlockwise()));
}

void MainWindow::loadPlugins()
{
    Component *andGate = new AndGate(this);
    m_ui->treeWidget->addComponent(andGate);

    Component *orGate = new OrGate(this);
    m_ui->treeWidget->addComponent(orGate);

    Component *button = new Button(this);
    m_ui->treeWidget->addComponent(button);
}


