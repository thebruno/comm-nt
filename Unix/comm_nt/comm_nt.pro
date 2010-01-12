# -------------------------------------------------
# Project created by QtCreator 2009-10-13T23:01:36
# -------------------------------------------------
TARGET = comm_nt
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    ReceiverThread.cpp \
    Utilities.cpp \
    stdafx.cpp \
    Socket.cpp \
    CommonClasses.cpp \
    Client.cpp \
    loginform.cpp \
    chat.cpp
HEADERS += mainwindow.h \
    ReceiverThread.h \
    Utilities.h \
    targetver.h \
    stdafx.h \
    Socket.h \
    CommonClasses.h \
    Client.h \
    BasicSystemClasses.h \
    loginform.h \
    chat.h \
    semaphore.h \
    pthread.h
FORMS += mainwindow.ui \
    loginform.ui \
    chat.ui
LIBS += -L/ \
    -lWS2_32 \
    -LD:\pthreadVC2.lib
RESOURCES += resources.qrc
