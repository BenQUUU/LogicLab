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
    Scene *m_scene;
    ComponentItem *m_item;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(Scene *scene, const QList<ComponentItem*> &items);

    void undo();
    void redo();

private:
    Scene *m_scene;
    QList<ComponentItem*> m_items;
};

class RotateCommand : public QUndoCommand
{
public:
    RotateCommand(const QList<ComponentItem*> &items, const double &angle);

    void undo();
    void redo();

private:
    QList<ComponentItem*> m_items;
    double m_angle;
};
