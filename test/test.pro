include(gtest_dependency.pri)
QT += widgets
QT += testlib
TEMPLATE = app
CONFIG += c++1z
CONFIG += thread
CONFIG += qt
LIBS += "-LC:/boost_1_74_0/stage/lib/"

HEADERS += \
        ../src/Utility.h \
        ../src/Message.h \
        message_test.h \
        validator_test.h

SOURCES += \
        ../src/Message.cpp \
        testmain.cpp

INCLUDEPATH += \
        ../src \
        C:/boost_1_74_0/
