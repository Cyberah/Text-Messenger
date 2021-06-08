QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++1z
CONFIG += qt
TARGET = TextMessenger
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += C:/boost_1_74_0/
LIBS += "-LC:/boost_1_74_0/stage/lib/"

SOURCES += \
    Client.cpp \
    KeyboardResponsiveTextEdit.cpp \
    PushButtonHover.cpp \
    Server.cpp \
    Service.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Client.h \
    Client_Session.h \
    KeyboardResponsiveTextEdit.h \
    PushButtonHover.h \
    Server.h \
    Server_Session.h \
    Service.h \
    Utility.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
