QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commands.cpp \
    componentlist.cpp \
    componentmime.cpp \
    components/andgate.cpp \
    components/button.cpp \
    components/orgate.cpp \
    core/component.cpp \
    core/componentitem.cpp \
    core/inputpin.cpp \
    core/outputpin.cpp \
    core/pinitem.cpp \
    core/wire.cpp \
    main.cpp \
    mainwindow.cpp \
    componenttree.cpp \
    scene.cpp

HEADERS += \
    commands.hpp \
    componentlist.hpp \
    componentmime.hpp \
    components/andgate.hpp \
    components/button.hpp \
    components/orgate.hpp \
    core/component.hpp \
    core/componentitem.hpp \
    core/inputpin.hpp \
    core/outputpin.hpp \
    core/pinitem.hpp \
    core/wire.hpp \
    mainwindow.hpp \
    componenttree.hpp \
    scene.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
