#pragma once

#include <QUndoCommand>
#include <QPointF>

class ComponentItem;
class Scene;

class AddCommand : public QUndoCommand
{
public:
    AddCommand(Scene *scene, ComponentItem *item);

    void undo();
    void redo();

private:
    Scene *_scene;
    ComponentItem *_item;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(Scene *scene, const QList<ComponentItem*> &items);

    void undo();
    void redo();

private:
    Scene *_scene;
    QList<ComponentItem*> _items;
};

class RotateCommand : public QUndoCommand
{
public:
    RotateCommand(const QList<ComponentItem*> &items, const double &angle);

    void undo();
    void redo();

private:
    QList<ComponentItem*> _items;
    double _angle;
};
