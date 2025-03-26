#include "componenttree.hpp"
#include "componentlist.hpp"
#include "core/component.hpp"

#include <QApplication>
#include <QMenu>

ComponentDelegate::ComponentDelegate(QTreeWidget* treeWidget) :
    _treeWidget(treeWidget)
{ }

void ComponentDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QAbstractItemModel* model = index.model();

    if (!model->parent(index).isValid()) {
        QStyleOptionButton buttonOption;

        buttonOption.state = option.state;
        buttonOption.state &= ~QStyle::State_HasFocus;

        buttonOption.rect = option.rect;
        buttonOption.palette = option.palette;
        buttonOption.features = QStyleOptionButton::None;
        _treeWidget->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, _treeWidget);

        QStyleOption branchOption;
        static constexpr int i = 9;
        QRect r = option.rect;
        branchOption.rect = QRect(r.left() + i/2, r.top() + (r.height() - i)/2, i, i);
        branchOption.palette = option.palette;
        branchOption.state = QStyle::State_Children;

        if (_treeWidget->isExpanded(index)) branchOption.state |= QStyle::State_Open;

        _treeWidget->style()->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, painter, _treeWidget);

        QRect textrect = QRect(r.left() + i * 2, r.top(), r.width() - ((5*i)/2), r.height());
        QString originalText = index.model()->data(index, Qt::DisplayRole).toString();

        QString elidedText = option.fontMetrics.elidedText(originalText, Qt::ElideMiddle, option.rect.width());
        _treeWidget->style()->drawItemText(painter, textrect, Qt::AlignCenter, option.palette, _treeWidget->isEnabled(), elidedText);
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}

QSize ComponentDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    return QItemDelegate::sizeHint(opt, index) + QSize(2, 2);
}

ComponentTree::ComponentTree(QWidget *parent) :
    QTreeWidget(parent),
    _delegate(new ComponentDelegate(this)),
    _viewMode(QListView::IconMode)
{
    setItemDelegate(_delegate);

    connect(this, SIGNAL(itemPressed(QTreeWidgetItem*, int)), SLOT(handleMousePress(QTreeWidgetItem*)));
}

ComponentTree::~ComponentTree()
{
    delete _delegate;
    _delegate = nullptr;
}

void ComponentTree::addComponent(Component* component)
{
    const QString componentName = component->metaObject()->className();
    if (_components.contains(componentName)) return;

    _components.insert(componentName, component);

    const QString categoryName = component->category();
    QTreeWidgetItem *categoryItem = _categories.value(categoryName);

    if (!categoryItem) {
        categoryItem = new QTreeWidgetItem();
        categoryItem->setText(0, categoryName);
        addTopLevelItem(categoryItem);

        ComponentList *listWidget = new ComponentList();
        listWidget->setViewMode(_viewMode);

        QTreeWidgetItem *listItem = new QTreeWidgetItem(categoryItem);
        setItemWidget(listItem, 0, listWidget);


        _categories.insert(categoryName, categoryItem);
    }

    QTreeWidgetItem *childItem = categoryItem->child(0);

    if (!childItem) return;

    ComponentList *listWidget = qobject_cast<ComponentList*>(itemWidget(childItem, 0));

    if (!listWidget) return;

    listWidget->addComponent(component);

    adjustListHeight(categoryItem);
}

Component *ComponentTree::component(const QString &name)
{
    return _components.value(name);
}

void ComponentTree::setViewMode(const QListView::ViewMode &mode)
{
    _viewMode = mode;
    updateLists();
}

void ComponentTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;

    QAction *iconViewAction = menu.addAction(tr("Icon view"), this, SLOT(setIconMode()));
    iconViewAction->setCheckable(true);
    iconViewAction->setChecked(_viewMode == QListView::IconMode);

    QAction *listViewAction = menu.addAction(tr("List view"), this, SLOT(setListMode()));
    listViewAction->setCheckable(true);
    listViewAction->setChecked(_viewMode == QListView::ListMode);

    menu.exec(event->globalPos());
}

void ComponentTree::resizeEvent(QResizeEvent *event)
{
    QTreeWidget::resizeEvent(event);

    for (int i = 0; i < topLevelItemCount(); ++i)
        adjustListHeight(topLevelItem(i));
}

void ComponentTree::handleMousePress(QTreeWidgetItem *item)
{
    if (!(qApp->mouseButtons() & Qt::LeftButton)) return;

    if (!item) return;

    if (item->parent()) return;

    item->setExpanded(!item->isExpanded());
}

void ComponentTree::setIconMode()
{
    setViewMode(QListView::IconMode);
}

void ComponentTree::setListMode()
{
    setViewMode(QListView::ListMode);
}

void ComponentTree::updateLists()
{
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = topLevelItem(i);

        if (ComponentList *list = qobject_cast<ComponentList*>(itemWidget(item->child(0), 0))) {
            list->setViewMode(_viewMode);
        }

        adjustListHeight(topLevelItem(i));
    }

    updateGeometries();
}

void ComponentTree::adjustListHeight(QTreeWidgetItem *item)
{
    QTreeWidgetItem *childItem = item->child(0);
    if (!childItem) return;

    if (ComponentList *listWidget = qobject_cast<ComponentList*>(itemWidget(childItem, 0))) {
        listWidget->doItemsLayout();

        const int height = qMax(listWidget->contentsSize().height() ,1);
        listWidget->setFixedHeight(height);
        childItem->setSizeHint(0, QSize(-1, height - 1));
    }
}
