TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    select_server.c \
    wrap.c \
    global.c \
    link_queue.c \
    mes_thread.c \
    sql.c \
    userinfo.c \
    onlineusers.c

HEADERS += \
    server.h \
    wrap.h \
    global.h \
    link_queue.h \
    mes_thread.h \
    sql.h \
    userinfo.h \
    onlineusers.h

QMAKE_CXXFLAGS += -std=c++0x -pthread

include(/usr/include/mysql/)
LIBS += -pthread
LIBS +=-lmysqlclient

