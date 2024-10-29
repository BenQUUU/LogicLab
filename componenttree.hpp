#pragma once

#include <QItemDelegate>
#include <QTreeWidget>
#include <QListView>

class Component;

class ComponentDelegate : public QItemDelegate
{
public:
    ComponentDelegate(QTreeWidget* treeWidget);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    QTreeWidget* _treeWidget;
};

class ComponentTree : public QTreeWidget
{
    Q_OBJECT

public:
    ComponentTree(QWidget* parent = nullptr);
    ~ComponentTree();

    void addComponent(Component* component);
    Component* component(const QString& name);

    void setViewMode(const QListView::ViewMode& mode);
    inline QListView::ViewMode viewMode() const { return _viewMode; }

protected:
    void contextMenuEvent(QContextMenuEvent* event);
    void resizeEvent(QResizeEvent* event);

private slots:
    void handleMousePress(QTreeWidgetItem* item);
    void setIconMode();
    void setListMode();
    void updateLists();

private:
    void adjustListHeight(QTreeWidgetItem* item);

private:
    QHash<QString, QTreeWidgetItem*> _categories;
    QHash<QString, Component*> _components;
    ComponentDelegate* _delegate;
    QListView::ViewMode _viewMode;
};
