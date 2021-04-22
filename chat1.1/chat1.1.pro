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
    sql.c \
    userinfo.c \
    onlineusers.c \
    epoll_server.c \
    thread_pool.c

HEADERS += \
    server.h \
    wrap.h \
    global.h \
    link_queue.h \
    sql.h \
    userinfo.h \
    onlineusers.h \
    thread_pool.h

QMAKE_CXXFLAGS += -std=c++0x -pthread

include(/usr/include/mysql/)
LIBS += -pthread
LIBS +=-lmysqlclient

SUBDIRS += \
    chat1.1.pro

