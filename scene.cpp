#include "scene.hpp"

#include "componentmime.hpp"
#include "core/component.hpp"
#include "core/outputpin.hpp"
#include "core/inputpin.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QDragEnterEvent>
#include <QGraphicsView>
#include <QApplication>
#include <QScrollBar>
#include <QUndoStack>
#include <QPainter>
#include <QDebug>
#include "commands.hpp"

Scene::Scene() :
    _wire(0),
    _undoStack(new QUndoStack),
    _modified(false)
{
    setGridSize(15);

    connect(_undoStack, SIGNAL(canRedoChanged(bool)), SIGNAL(canRedoChanged(bool)));
    connect(_undoStack, SIGNAL(canUndoChanged(bool)), SIGNAL(canUndoChanged(bool)));
    connect(_undoStack, SIGNAL(cleanChanged(bool)), SLOT(setModified(bool)));
}

Scene::~Scene()
{
    delete _undoStack;
    _undoStack = nullptr;
}

QList<ComponentItem*> Scene::selectedComponents()
{
    QList<ComponentItem*> components;

    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == Wire::Type) continue;

        if (ComponentItem *component = dynamic_cast<ComponentItem*>(item)) components << component;
    }

    return components;
}

QList<Wire*> Scene::selectedWires()
{
    QList<Wire*> wires;

    foreach (QGraphicsItem *item, selectedItems()) {
        if (Wire *wire = dynamic_cast<Wire*>(item))
            wires << wire;
    }

    return wires;
}

ComponentItem *Scene::selectedComponent()
{
    QList<ComponentItem*> components = selectedComponents();
    if (!components.count()) return nullptr;

    return components.first();
}

int Scene::gridSize() const
{
    return property("gridSize").toInt();
}

void Scene::setModified(bool modified)
{
    if (_modified == modified) return;

    if(!modified) _undoStack->setClean();

    _modified = modified;
    emit modificationChanged(_modified);
}

bool Scene::canUndo() const
{
    return _undoStack->canUndo();
}

bool Scene::canRedo() const
{
    return _undoStack->canRedo();
}

void Scene::deleteSelection()
{
    foreach (Wire *wire, selectedWires()) {
        removeItem(wire);
        delete wire;
    }

    foreach (ComponentItem *component, selectedComponents()) {
        removeItem(component);
        delete component;
    }

    _undoStack->push(new DeleteCommand(this, selectedComponents()));
}

void Scene::showProperties()
{
    if (ComponentItem *item = selectedComponent()) item->showProperties();
}

void Scene::clear()
{
    _undoStack->clear();

    QGraphicsScene::clear();
}

void Scene::setGridSize(const int &size)
{
    setProperty("gridSize", size);
}

void Scene::rotateClockwise()
{
    _undoStack->push(new RotateCommand(selectedComponents(), 90));
}

void Scene::rotateAnticlockwise()
{
    _undoStack->push(new RotateCommand(selectedComponents(), -90));
}

void Scene::undo()
{
    _undoStack->undo();
}

void Scene::redo()
{
    _undoStack->redo();
}

inline float nearestStep(const float &val, const int &step) {
    int tmp = qRound(val) + step / 2;
    tmp -= tmp % step;
    return tmp;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    if (event->buttons() & Qt::LeftButton) {
        foreach (QGraphicsItem *pressItem, items(event->scenePos())) {
            InputPin *input = dynamic_cast<InputPin*>(pressItem);
            OutputPin *output = dynamic_cast<OutputPin*>(pressItem);

            if (!input && !output) continue;

            if (input && input->wire()) continue;

            const QPointF center = pressItem->scenePos() + pressItem->boundingRect().center();
            _wire = new Wire(center, event->scenePos());
            addItem(_wire);

            break;
        }
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if (event->buttons() & Qt::LeftButton) {
        if (!_wire) return;

        _wire->setEndPos(event->scenePos());
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    //TODO
    if(_wire) {
        const QPointF startPos = _wire->startPos();
        const QPointF endPos = _wire->endPos();

        removeItem(_wire);
        delete _wire;
        _wire = nullptr;

        //start item
        QGraphicsItem *startItem = 0;
        foreach (QGraphicsItem *item, items(startPos)) {
            if ((item->type() != InputPin::Type) && (item->type() != OutputPin::Type)) continue;

            startItem = item;
            break;
        }

        if (!startItem) return;

        //end item
        QGraphicsItem *endItem = 0;
        foreach (QGraphicsItem *item, items(endPos)) {
            if ((item->type() != InputPin::Type) && (item->type() != OutputPin::Type)) continue;

            if (item->type() == startItem->type()) continue;

            endItem = item;
            break;
        }

        if (!endItem) return;

        //resolve types
        OutputPin *output = (startItem->type() == OutputPin::Type) ? dynamic_cast<OutputPin*>(startItem) : dynamic_cast<OutputPin*>(endItem);
        InputPin *input   = (startItem->type() == InputPin::Type)  ? dynamic_cast<InputPin*>(startItem)  : dynamic_cast<InputPin*>(endItem);

        if (input->wire()) return;

        Wire *wire = new Wire(input, output);
        wire->setZValue(-1);
        addItem(wire);
    }
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const ComponentMime *mimeData = qobject_cast<const ComponentMime*>(event->mimeData());

    if(!mimeData) return;

    Component *component = mimeData->component();
    ComponentItem *dragItem = component->item();

    const QRectF boundingRect = dragItem->boundingRect();
    QPoint targetPos = (event->scenePos() - boundingRect.center()).toPoint();

    dragItem->setPos(targetPos);
    dragItem->setTransformOriginPoint(boundingRect.center());

    _undoStack->push(new AddCommand(this, dragItem));
    setModified(true);
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (qobject_cast<const ComponentMime*>(event->mimeData())) {
        event->acceptProposedAction();
        return;
    }

    QGraphicsScene::dragMoveEvent(event);
}
