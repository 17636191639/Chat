TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    config.cpp \
    msgserver.cpp \
    msgsocket.cpp \
    main.cpp \
    global.cpp

HEADERS += \
    global.h \
    config.h \
    msgserver.h \
    msgsocket.h

QMAKE_CXXFLAGS += -std=c++0x -pthread

include(database/database.pri)
include(data_structure/data_structure.pri)
include(info/info.pri)
include(reactor/reactor.pri)
include(thread_pool/thread_pool.pri)

include(/usr/include/mysql/)
LIBS += -pthread
LIBS +=-lmysqlclient
