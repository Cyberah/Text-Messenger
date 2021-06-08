include(gtest_dependency.pri)
QT += widgets
QT += testlib
TEMPLATE = app
CONFIG += c++1z
CONFIG += thread
CONFIG += qt
LIBS += "-LC:/boost_1_74_0/stage/lib/"

HEADERS += \
        ../src/Client.h \
        ../src/Server.h \
        ../src/Service.h \
        ../src/Utility.h \
        ../src/Server_Session.h \
        tst_client.h

SOURCES += \
        ../src/Client.cpp \
        ../src/Server.cpp \
        ../src/Service.cpp \
        testmain.cpp

INCLUDEPATH += \
        ../src \
        C:/boost_1_74_0/
