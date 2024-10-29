#include "commands.hpp"
#include "scene.hpp"
#include "core/componentitem.hpp"

AddCommand::AddCommand(Scene *scene, ComponentItem *item) :
    m_scene(scene),
    m_item(item)
{
}

void AddCommand::undo()
{
    m_scene->removeItem(m_item);
}

void AddCommand::redo()
{
    m_scene->addItem(m_item);
}

DeleteCommand::DeleteCommand(Scene *scene, const QList<ComponentItem *> &items) :
    m_scene(scene),
    m_items(items)
{

}

void DeleteCommand::undo()
{
    foreach(ComponentItem *item, m_items) {
        m_scene->addItem(item);
    }
}

void DeleteCommand::redo()
{
    foreach(ComponentItem *item, m_items) {
        m_scene->removeItem(item);
    }
}

RotateCommand::RotateCommand(const QList<ComponentItem*> &items, const double &angle) :
    m_items(items),
    m_angle(angle)
{
}

void RotateCommand::undo()
{
    foreach(ComponentItem *item, m_items) {
        if(!item)
            continue;

        item->setRotation(item->rotation() - m_angle);
    }
}

void RotateCommand::redo()
{
    foreach(ComponentItem *item, m_items) {
        if(!item)
            continue;

        item->setRotation(item->rotation() + m_angle);
    }
}
