#include "commands.hpp"
#include "scene.hpp"
#include "core/componentitem.hpp"

AddCommand::AddCommand(Scene *scene, ComponentItem *item) :
    _scene(scene),
    _item(item)
{ }

void AddCommand::undo()
{
    _scene->removeItem(_item);
}

void AddCommand::redo()
{
    _scene->addItem(_item);
}

DeleteCommand::DeleteCommand(Scene *scene, const QList<ComponentItem *> &items) :
    _scene(scene),
    _items(items)
{ }

void DeleteCommand::undo()
{
    foreach (ComponentItem *item, _items) {
        _scene->addItem(item);
    }
}

void DeleteCommand::redo()
{
    foreach (ComponentItem *item, _items) {
        _scene->removeItem(item);
    }
}

RotateCommand::RotateCommand(const QList<ComponentItem*> &items, const double &angle) :
    _items(items),
    _angle(angle)
{ }

void RotateCommand::undo()
{
    foreach (ComponentItem *item, _items) {
        if(!item) continue;

        item->setRotation(item->rotation() - _angle);
    }
}

void RotateCommand::redo()
{
    foreach (ComponentItem *item, _items) {
        if(!item) continue;

        item->setRotation(item->rotation() + _angle);
    }
}
