#pragma once

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QTimer>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QTimer>

class ComponentItem;
class QUndoStack;
class Wire;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene();
    ~Scene();

    QList<ComponentItem*> selectedComponents();
    QList<Wire*> selectedWires();
    ComponentItem *selectedComponent();

    int gridSize() const;

    bool isModified() const { return _modified; }

    bool canUndo() const;
    bool canRedo() const;

signals:
    void modificationChanged(bool modified);
    void canRedoChanged(bool canRedo);
    void canUndoChanged(bool canUndo);

public slots:
    void deleteSelection();
    void showProperties();
    void clear();

    void setGridSize(const int &size);
    void setModified(bool modified);

    void rotateClockwise();
    void rotateAnticlockwise();

    void undo();
    void redo();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

private:
    Wire *_wire;
    QUndoStack *_undoStack;
    bool _modified;
};

