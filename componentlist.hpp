#pragma once

#include <QDragEnterEvent>
#include <QListWidget>
#include <QMimeData>
#include <QDrag>

class Component;

class ComponentList : public QListWidget
{
    Q_OBJECT

public:
    ComponentList(QWidget *parent = 0);

    void addComponent(Component *component);

    using QListView::contentsSize;

protected:
    void mouseMoveEvent(QMouseEvent *event);

private:
    QHash<QListWidgetItem*, Component*> _components;
};
