#pragma once

#include <QMimeData>

class Component;

class ComponentMime : public QMimeData
{
    Q_OBJECT

public:
    ComponentMime();

    inline void setComponent(Component* component) { _component = component; }
    inline Component* component() const { return _component; }

private:
    Component* _component;
};
